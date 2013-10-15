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

struct nhpc_queue_elem_t {
   void              *data;
   nhpc_queue_elem_t *next;
   nhpc_queue_elem_t *prev;
};

struct nhpc_queue_t {
   nhpc_uint_t         max;
   nhpc_uint_t         count;
   nhpc_queue_elem_t  *head;
   nhpc_queue_elem_t  *tail;
   nhpc_queue_elem_t  *elements;
   
   pthread_mutex_t     mutex;
   pthread_cond_t      cond;
};

nhpc_queue_t *nhpc_init_queue(nhpc_pool_t *p, int size);
nhpc_queue_t *nhpc_init_queue(int size);
void         *nhpc_get_queue(nhpc_queue_t *q);
void          nhpc_insert_queue(nhpc_queue_t *q, void *data);
bool          nhpc_check_queueempty(nhpc_queue_t *q);
bool          nhpc_check_queuefull(nhpc_queue_t *q);



