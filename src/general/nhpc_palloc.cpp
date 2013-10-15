/*
 *	(C) 2013 Varun Mittal <varunmittal91@gmail.com>
 *	NeweraHPC program is distributed under the terms of the GNU General Public License v3
 *
 *	This file is part of NeweraHPC.
 *
 *	NeweraHPC is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation version 3 of the License.
 *
 *	NeweraHPC is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with NeweraHPC.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef __linux__
#include <malloc.h>
#endif
#include <string.h>

#include <include/nhpc_general.h>

using namespace std;

nhpc_pool_t *nhpc_create_pool(nhpc_size_t size) {
   
   size = size * NHPC_DEFAULT_POOL_SIZE;
   nhpc_pool_t *p = (nhpc_pool_t *)nhpc_alloc(size);
   
   if(!p) {
      nhpc_log_error("ERROR: create_pool() failed, size:%iBytes", size);
      return NULL;
   }
   
   nhpc_log_debug1(LOG_LEVEL_DEBUG_5, "DEBUG: created memory pool with size:%i Bytes, address:%p\n", size, p);
   
   p->d.last   = (u_char *)p + sizeof(nhpc_pool_t);
   p->d.end    = (u_char *)p + size;
   p->d.next   = NULL;
   p->d.failed = 0;
   
   size   = size - sizeof(nhpc_pool_t);
   p->max = (size < NHPC_MAX_ALLOC_FROM_POOL) ? size : NHPC_MAX_ALLOC_FROM_POOL;
   
   p->current = p;
   
   return p;
}

void nhpc_destroy_pool(nhpc_pool_t *pool) {
   nhpc_pool_t        *p, *n;
   nhpc_pool_large_t  *l;
   
   for(l = pool->large; l; l = l->next) {
      if(l->alloc)
	 nhpc_free(l->alloc);
   }
   
   for(p = pool, n = p->d.next; ; p = n, n = n->d.next) {
      nhpc_free(p);
      
      if(!n)
	 break;
   }
}

void nhpc_reset_pool(nhpc_pool_t *pool)
{
   nhpc_pool_t        *p;
   nhpc_pool_large_t  *l;
   
   for(l = pool->large; l; l = l->next) {
      if(l->alloc) {
	 nhpc_free(l->alloc);
	 nhpc_log_debug0(LOG_LEVEL_DEBUG_5, "DEBUG: Deleting large memory block with address:%p\n", l->alloc);
      }
   }
   
   for(p = pool; p; p = p->d.next) {
      p->d.last = (u_char *)p + sizeof(nhpc_pool_t);
      
      nhpc_log_debug0(LOG_LEVEL_DEBUG_5, "DEBUG: reset memory pool with address:%p\n", p);
   }
}

void *nhpc_palloc(nhpc_pool_t *pool, nhpc_size_t size)
{
   u_char       *m;
   nhpc_pool_t  *p;
   
   if(size <= pool->max)
   {
      p = pool->current;
      
      do {
	 m = p->d.last;
	 if((nhpc_size_t)(p->d.end - m) >= size) {
	    p->d.last = m + size;
	    
	    nhpc_log_debug1(LOG_LEVEL_DEBUG_5, "DEBUG: Created memory block with address:%p and size:%i \n", m, size);
	    
	    return m;
	 }
	 
	 p = p->d.next;
	 
      } while(p);
      
      return nhpc_palloc_block(pool, size);
   }
   
   return nhpc_palloc_large(pool, size);
}

void *nhpc_palloc_block(nhpc_pool_t *pool, nhpc_size_t size)
{
   u_char      *m;
   nhpc_size_t  psize;
   nhpc_pool_t *newp, *p, *current;

   psize = (nhpc_size_t) (pool->d.end - (u_char *) pool);
   m     = (u_char *)nhpc_alloc(psize);
   if(!m)
      return NULL;
   
   newp = (nhpc_pool_t *)m;
   
   newp->d.end    = m + psize;
   newp->d.next   = NULL;
   newp->d.failed = 0;
   
   m += sizeof(nhpc_pool_data_t);
   newp->d.last = m + size;
   
   current = pool->current;
   for(p = current; p->d.next; p = p->d.next) {
      if(p->d.failed++ > 4)
	 current = p->d.next;    
   }
   
   p->d.next = newp;
   
   pool->current = current ? current : newp;
   
   nhpc_log_debug1(LOG_LEVEL_DEBUG_5, "DEBUG: Created pool memory block with address:%p and size:%i \n", m, size);
   
   return m;
}

void *nhpc_palloc_large(nhpc_pool_t *pool, nhpc_size_t size)
{
   void              *p;
   nhpc_uint_t        n;
   nhpc_pool_large_t *large;
   
   p = nhpc_alloc(size);
   if(!p)
      return NULL;
   
   n = 0;
   
   for(large = pool->large; large; large = large->next) {
      if(!(large->alloc)) {
	 large->alloc = p;
	 return p;
      }
      
      if(n++ > 3)
	 break;
   }
   
   large = (nhpc_pool_large_t *)nhpc_palloc(pool, sizeof(nhpc_pool_large_t));
   if(!large)
   {
      nhpc_free(p);
      return NULL;
   }
   
   large->alloc = p;
   large->next  = pool->large;
   pool->large  = large;
   
   nhpc_log_debug1(LOG_LEVEL_DEBUG_5, "DEBUG: Created large memory block with address:%p and size:%i \n", p, size);
   
   return p;
}

nhpc_status_t nhpc_pfree(nhpc_pool_t *pool, void *p)
{
   nhpc_pool_large_t  *l;
   
   for(l = pool->large; l; l = l->next) {
      if(p == l->alloc) {
	 nhpc_free(l->alloc);
	 l->alloc = NULL;
	 
	 return NHPC_SUCCESS;
      }
   }
   
   return NHPC_FAIL;
}

void *nhpc_pcalloc(nhpc_pool_t *pool, nhpc_size_t size) {
   void *p = nhpc_palloc(pool, size);
   
   if(p)
      memset(p, 0, size);
   
   return p;
}
