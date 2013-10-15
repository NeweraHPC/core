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

nhpc_stack_t *nhpc_init_stack(nhpc_uint_t size, nhpc_pool_t *pool) {
   
   nhpc_stack_t *stack;

   if(pool) {
      stack           = (nhpc_stack_t *)nhpc_palloc(pool, size);
      stack->elements = (nhpc_stack_elem_t *)nhpc_palloc(pool, sizeof(nhpc_stack_elem_t) * size);
   } else {
      stack           = (nhpc_stack_t *)nhpc_alloc(size);
      stack->elements = (nhpc_stack_elem_t *)nhpc_alloc(sizeof(nhpc_stack_elem_t) * size);
   }
   
   if(!stack || !stack->elements) {
      nhpc_log_error("ERROR: init_stack() failed");
      return NULL;
   }
   
   pthread_mutex_init(&stack->mutex, NULL);
   pthread_cond_init(&stack->cond, NULL);
   
   stack->max   = size;
   stack->count = 0;
   
   return stack;
}

nhpc_stack_elem_t *_nhpc_get_free_stackelem(nhpc_stack_t *stack) {

   for(int i = 0; i < stack->max; i++) {
      if(!stack->elements[i].data)
	 return &stack->elements[i];
   }
   
   return NULL;
}

void *nhpc_pop_stack(nhpc_stack_t *stack) {

   void *d = NULL;
   pthread_mutex_lock(&stack->mutex);
   while(!stack->head)
      pthread_cond_wait(&stack->cond, &stack->mutex);
   
   d = stack->head->data;
   stack->head->data = NULL;
   stack->head       = stack->head->next;
   
   stack->count--;
   
   pthread_cond_signal(&stack->cond);
   pthread_mutex_unlock(&stack->mutex);
   
   return d;
}

void nhpc_push_stack(nhpc_stack_t *stack, void *data) {
   
   pthread_mutex_lock(&stack->mutex);

   while(stack->count == stack->max)
      pthread_cond_wait(&stack->cond, &stack->mutex);
   
   nhpc_stack_elem_t *elem = _nhpc_get_free_stackelem(stack);
   elem->data = data;
   
   if(stack->head)
      elem->next = stack->head;
   stack->head = elem;
   
   stack->count++;

   pthread_cond_signal(&stack->cond);
   pthread_mutex_unlock(&stack->mutex);
}