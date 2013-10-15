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

#include <string.h>

#include <include/nhpc_general.h>

using namespace std;

nhpc_buffer_t *nhpc_buffer_init(nhpc_pool_t *p) {
   nhpc_buffer_t *buffer = (nhpc_buffer_t *)nhpc_pcalloc(p, sizeof(nhpc_buffer_t));

   buffer->current = NULL;
   buffer->head    = NULL;
   buffer->pool    = p;
   buffer->d.next  = NULL;
   buffer->d.max   = 0;
   
   return buffer;
}

void nhpc_buffer_destroy(nhpc_buffer_t *buffer) {
   
}

void nhpc_buffer_add_data(nhpc_buffer_t *buffer, u_char *address, nhpc_size_t data_len, 
			  nhpc_uint_t buffer_data_type, nhpc_uint_t deallocate) {
   
   nhpc_buffer_data_t *buffer_data = buffer->current;
   if(!buffer->current) {
      buffer->current = &(buffer->d);
      buffer->head    = &buffer->d;      
   } else {
      buffer->current->next = (nhpc_buffer_data_t *)nhpc_palloc(buffer->pool, sizeof(nhpc_buffer_data_t));
      buffer->current       = buffer->current->next;
   }
   buffer_data = buffer->current;
   if(!buffer->head)
      buffer->head = buffer_data;

   buffer_data->data_type = buffer_data_type;
   buffer_data->next      = NULL;
   
   if(buffer_data_type & NHPC_BUFFER_DATA_MEM_BLOCK) {

      if(deallocate) {
	 u_char *_address = (u_char *)nhpc_pcalloc(buffer->pool, data_len);
	 memcpy(_address, address, data_len);
	 address = _address;	 
	 
	 buffer_data->deallocate = 1;
      }
      
      buffer_data->start = address;
      buffer_data->end   = address + data_len;
   } else if(buffer_data_type & NHPC_BUFFER_DATA_FILE) {
      
      buffer_data->address = address;
      buffer_data->readpos = 0;
   }
      
   return;
}

void nhpc_buffer_add_header_data(nhpc_buffer_t *buffer, u_char *address, nhpc_size_t data_len, 
				 nhpc_uint_t buffer_data_type, nhpc_uint_t deallocate) {
   

   nhpc_log_debug1(LOG_LEVEL_DEBUG_4, "DEBUG: %s:%i\n", "adding header data to buffer, data len", data_len);
   
   nhpc_buffer_data_t *buffer_head;
   
   if(!buffer->head) {
      buffer->head = &buffer->d;
      
      buffer_head  = buffer->head;
      buffer_head->next = NULL;
      
      nhpc_log_debug0(LOG_LEVEL_DEBUG_4, "DEBUG: %s\n", "setting buffer_head to buffer->d");
      
   } else {
      buffer_head = (nhpc_buffer_data_t *)nhpc_palloc(buffer->pool, sizeof(nhpc_buffer_data_t));
      buffer_head->next = buffer->head;
      buffer->head = buffer_head;
   }
   
   if(deallocate) {
      u_char *_address = (u_char *)nhpc_palloc(buffer->pool, data_len);
      memcpy(_address, address, data_len);
      address = _address;
      
      buffer->d.deallocate = 1;
   }

   buffer_head->start = address;
   buffer_head->end   = address + data_len;
}
