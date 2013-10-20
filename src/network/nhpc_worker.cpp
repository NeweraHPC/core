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

#include <include/neweraHPC.h>

nhpc_worker_pool_t *worker_pool = NULL;

nhpc_worker_t *nhpc_create_worker(nhpc_pool_t *p);
void          *nhpc_worker_thread(nhpc_worker_t *w);

nhpc_status_t nhpc_init_workers(nhpc_config_t *config) {
   nhpc_uint_t count = 1;
   
   nhpc_str_t *worker_count_str = ( nhpc_str_t * )nhpc_rbtree_search(config->options, "workers");
   if( worker_count_str ) {
      count = nhpc_atoi(worker_count_str->data);
   }
   nhpc_log_debug0(LOG_LEVEL_DEBUG_4, "DEBUG: Initializing worker pool:%i\n", count);
   nhpc_log_debug0(LOG_LEVEL_DEBUG_4, "DEBUG: Setting worker pool count:%i\n", count);
   
   if( !nhpc_init_worker_pool(config->service_pool, count) ) {
      nhpc_log_error("ERROR: failed to initialize worker\n");
      exit(0);
   }
   
   return NHPC_SUCCESS;
}

void *nhpc_init_worker_pool(nhpc_pool_t *p, nhpc_uint_t count) {
   worker_pool = (nhpc_worker_pool_t *)nhpc_palloc(p, sizeof(nhpc_worker_pool_t) * count);
   worker_pool->workers_queue = nhpc_init_queue(count);
   worker_pool->workers = (nhpc_worker_t **)nhpc_palloc(p, sizeof(nhpc_worker_t *) * count);
   
   nhpc_worker_t *worker;
   for(int i = 0; i < count; i++) {
      if( !(worker = nhpc_create_worker(p)) )
	 return NULL;
      worker_pool->workers[i] = worker;

      nhpc_insert_queue(worker_pool->workers_queue, worker);
   }
   
   return worker_pool;
}

nhpc_worker_t *nhpc_create_worker(nhpc_pool_t *p) {
   nhpc_worker_t *worker = (nhpc_worker_t *)nhpc_palloc(p, sizeof(nhpc_worker_t));
   
   pthread_mutex_init(&worker->mutex, NULL);
   pthread_cond_init(&worker->cond, NULL);
   worker->ev = NULL;
   
   if( nhpc_create_thread(&worker->tid, (void* (*)(void*))nhpc_worker_thread, worker) != NHPC_SUCCESS )
      return NULL;
   
   return worker;
}

void *nhpc_worker_thread(nhpc_worker_t *w) {
   pthread_mutex_lock(&w->mutex);
   while(1) {
      while(!w->ev)
	 pthread_cond_wait(&w->cond, &w->mutex);      
      w->ev->handler(w->ev);
      w->ev->available = 0;
      w->ev = NULL;
      nhpc_free_worker_to_pool(w);
   }
   pthread_mutex_unlock(&w->mutex);
}