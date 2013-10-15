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

using namespace std;

void _nhpc_init_queue(nhpc_queue_t *q, int size) {
   pthread_mutex_init(&q->mutex, NULL);
   pthread_cond_init(&q->cond, NULL);
   
   q->head     = NULL;
   q->tail     = NULL;
   q->count    = 0;
   q->max      = size;   
}

nhpc_queue_t *nhpc_init_queue(nhpc_pool_t *p, int size) {
   nhpc_queue_t *q = (nhpc_queue_t *)nhpc_palloc(p, sizeof(nhpc_queue_t));
   q->elements     = (nhpc_queue_elem_t *)nhpc_pcalloc(p, sizeof(nhpc_queue_elem_t) * size);
   
   _nhpc_init_queue(q, size);
   return q;
}

nhpc_queue_t *nhpc_init_queue(int size) {
   nhpc_queue_t *q = (nhpc_queue_t *)nhpc_alloc(sizeof(nhpc_queue_t));
   q->elements     = (nhpc_queue_elem_t *)nhpc_calloc(sizeof(nhpc_queue_elem_t) * size);
   
   _nhpc_init_queue(q, size);
   return q;
}

bool _nhpc_check_queuefull(nhpc_queue_t *q) {
   return (q->count == q->max);
}

bool _nhpc_check_queueempty(nhpc_queue_t *q) {
   return (q->count == 0);
}

nhpc_queue_elem_t *_nhpc_get_free_queueelem(nhpc_queue_t *q) {
   nhpc_queue_elem_t *qe;

   for(int i = 0; i < q->max; i++) {
      qe = &q->elements[i];
      if(!qe->data)
	 return qe;
   }
   
   return NULL;
}

bool nhpc_check_queuefull(nhpc_queue_t *q) {
   bool t;
   pthread_mutex_lock(&q->mutex);
   t = _nhpc_check_queuefull(q);
   pthread_mutex_unlock(&q->mutex);
   
   return t;
}

bool nhpc_check_queueempty(nhpc_queue_t *q) {
   bool t;
   pthread_mutex_lock(&q->mutex);
   t = _nhpc_check_queueempty(q);
   pthread_mutex_unlock(&q->mutex);
   
   return t;
}

void *nhpc_get_queue(nhpc_queue_t *q) {
   void *data;

   nhpc_queue_elem_t *qe;
   pthread_mutex_lock(&q->mutex);
   while(!q->head)
      pthread_cond_wait(&q->cond, &q->mutex);   
   
   qe       = q->head;
   data     = qe->data;
   qe->data = NULL;
   if(q->head == q->tail) {
      q->head = NULL;
      q->tail = NULL;
   }
   else {
      q->head       = q->head->next;
      q->head->prev = NULL;
   }
   q->count -= 1;
   
   pthread_cond_signal(&q->cond);
   pthread_mutex_unlock(&q->mutex);
   
   return data;
}

void nhpc_insert_queue(nhpc_queue_t *q, void *data) {
   pthread_mutex_lock(&q->mutex);
   while(_nhpc_check_queuefull(q)) {
      pthread_cond_wait(&q->cond, &q->mutex);
   }
   nhpc_queue_elem_t *qe = _nhpc_get_free_queueelem(q);
   if(!qe) {
      nhpc_log_error("ERROR: queue_insert() failed, no free element available");
      return;
   }

   qe->data = data;
   if(!q->head) {
      q->head = qe;
      q->tail = qe;
      
      qe->next = NULL;
      qe->prev = NULL;
   }
   else {
      q->tail->next = qe;
      qe->prev      = q->tail;
      qe->next      = NULL;
      q->tail       = qe;
   }
   q->count += 1;
   
   pthread_cond_signal(&q->cond);
   pthread_mutex_unlock(&q->mutex);
}