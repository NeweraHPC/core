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

#ifndef _NHPC_PALLOC_H_
#define _NHPC_PALLOC_H_

#define NHPC_MAX_ALLOC_FROM_POOL  (nhpc_pagesize - 1)
#define NHPC_DEFAULT_POOL_SIZE    (4 * 1024)

struct nhpc_pool_t;
struct nhpc_pool_data_t;
struct nhpc_pool_large_t;

struct nhpc_pool_data_t {
   u_char               *last;
   u_char               *end;
   nhpc_pool_t          *next;
   nhpc_uint_t           failed;
};

struct nhpc_pool_t {
   nhpc_pool_data_t       d;
   nhpc_size_t            max;
   nhpc_pool_t           *current;
   nhpc_pool_large_t     *large;
};

struct nhpc_pool_large_t {
   nhpc_pool_large_t     *next;
   void                 *alloc;
};

nhpc_pool_t *nhpc_create_pool(nhpc_size_t size);
void         nhpc_destroy_pool(nhpc_pool_t *pool);
void         nhpc_reset_pool(nhpc_pool_t *pool);

void *nhpc_palloc(nhpc_pool_t *pool, nhpc_size_t size);
void *nhpc_palloc_block(nhpc_pool_t *pool, nhpc_size_t size);
void *nhpc_palloc_large(nhpc_pool_t *pool, nhpc_size_t size);
void *nhpc_pcalloc(nhpc_pool_t *pool, nhpc_size_t size);

nhpc_status_t nhpc_pfree(nhpc_pool_t *pool, void *p);

#endif