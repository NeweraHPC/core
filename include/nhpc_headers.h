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

struct nhpc_headers_data_t {
   nhpc_uint_t   count;
   nhpc_uint_t   max;
   nhpc_size_t   datasize;
   u_char      **strings;
};

struct nhpc_headers_s {
   nhpc_headers_data_t  d;
   
   nhpc_headers_s  *next;
   nhpc_pool_t     *pool;   
};

nhpc_headers_t *nhpc_init_headers(nhpc_pool_t *pool, nhpc_uint_t max = 10);
void            nhpc_destroy_headers(nhpc_headers_t *headers);

#define nhpc_close_headers(headers) (nhpc_headers_add_string(headers, (u_char *)"\r\n"))
void nhpc_headers_add_string(nhpc_headers_t *headers, u_char *str);
void nhpc_headers_add_new_string(nhpc_headers_t *headers, u_char *str);
void nhpc_headers_add_string(nhpc_headers_t *headers, u_char *str1, u_char *str2);

void nhpc_headers_convert_to_buffer(nhpc_headers_t *headers, nhpc_buffer_t *buffer);