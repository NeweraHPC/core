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

nhpc_uint_t  nhpc_pagesize = 4096;
nhpc_uint_t  nhpc_pagesize_shift;
nhpc_uint_t  nhpc_cacheline_size;

void *nhpc_alloc(nhpc_size_t size) {
   void *p;
   p = malloc(size);
   if(!p)
      nhpc_log_error("ERROR: %s:%i\n", "malloc() failed for size", size);
   
   return p;
}

void *nhpc_calloc(nhpc_size_t size) {
   void *p = nhpc_alloc(size);
   if(p)
      memset(p, 0, size);
   
   return p;
}
