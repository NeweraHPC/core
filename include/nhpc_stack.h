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

#ifndef _NHPC_STACK_H_
#define _NHPC_STACK_H_

struct nhpc_stack_elem_t {
   void               *data;
   nhpc_stack_elem_t  *next;
};

struct nhpc_stack_t {
   nhpc_uint_t         max;
   nhpc_uint_t         count;
   
   nhpc_stack_elem_t  *head;
   nhpc_stack_elem_t  *elements;
   
   pthread_mutex_t     mutex;
   pthread_cond_t      cond;
};

nhpc_stack_t  *nhpc_init_stack(nhpc_uint_t size, nhpc_pool_t *pool = NULL);
void          *nhpc_pop_stack(nhpc_stack_t *stack);
void           nhpc_push_stack(nhpc_stack_t *stack, void *data);

#endif