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

extern nhpc_worker_pool_t *worker_pool;

struct nhpc_worker_s {
   pthread_mutex_t   mutex;
   pthread_cond_t    cond;
   nhpc_event_t     *ev;
   pthread_t         tid;
};

struct nhpc_worker_pool_s {
   nhpc_uint_t      count;
   nhpc_queue_t    *workers_queue;
   nhpc_worker_t  **workers;
};

#define nhpc_get_worker_from_pool() ((nhpc_worker_t *)nhpc_get_queue(worker_pool->workers_queue))
#define nhpc_free_worker_to_pool(w) (nhpc_insert_queue(worker_pool->workers_queue, w))
#define nhpc_add_event_worker(w, ev) \
pthread_mutex_lock(&w->mutex);       \
w->ev = ev;                          \
pthread_cond_signal(&w->cond);       \
pthread_mutex_unlock(&w->mutex);

nhpc_status_t nhpc_init_workers(nhpc_config_t *config);

void *nhpc_init_worker_pool(nhpc_pool_t *p, nhpc_uint_t count);
static inline void  nhpc_submit_job_worker_pool(nhpc_event_t *ev) {
   nhpc_worker_t *worker = nhpc_get_worker_from_pool();
   nhpc_add_event_worker(worker, ev);  
}

