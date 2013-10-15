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

#define NHPC_BUFFER_DATA_FILE      0x1
#define NHPC_BUFFER_DATA_MEM_BLOCK 0x2
#define NHPC_BUFFER_DEALLOCATE     0x4

#define MAX_BUFFER_SIZE 1000

struct nhpc_chain_t {
   nhpc_chain_t *next;
   u_char       *start;
   u_char       *end;
};

struct nhpc_buffer_data_t {
   FILE               *fp;
   u_char             *address;
   nhpc_uint_t         readpos;
   
   u_char             *start;
   u_char             *end;
   nhpc_size_t         max;
   
   nhpc_buffer_data_t *next;

   unsigned            deallocate:1;
   nhpc_uint_t         data_type;
};

struct nhpc_buffer_t {
   nhpc_buffer_data_t   d;
   nhpc_buffer_data_t  *current;
   nhpc_buffer_data_t  *head;
   
   nhpc_pool_t         *pool;
};

nhpc_buffer_t *nhpc_buffer_init(nhpc_pool_t *p);
void           nhpc_buffer_destroy(nhpc_buffer_t *buffer);
void           nhpc_buffer_add_data(nhpc_buffer_t *buffer, u_char *address, nhpc_size_t data_len, 
				    nhpc_uint_t buffer_data_type, nhpc_uint_t deallocate = 0);
void           nhpc_buffer_add_header_data(nhpc_buffer_t *buffer, u_char *address, nhpc_size_t data_len, 
					   nhpc_uint_t buffer_data_type, nhpc_uint_t deallocate = 0);


