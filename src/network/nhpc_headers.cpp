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

#include <include/neweraHPC.h>

nhpc_headers_t *nhpc_init_headers(nhpc_pool_t *pool, nhpc_uint_t max) {
   nhpc_headers_t *headers = (nhpc_headers_t *)nhpc_palloc(pool, sizeof(nhpc_headers_t));
   
   headers->d.datasize = 0;
   headers->d.count    = 0;
   headers->d.max      = max;
   headers->d.strings  = (u_char **)nhpc_palloc(pool, sizeof(u_char*) * max);
   headers->pool       = pool;
   headers->next       = NULL;
   
   return headers;
}

void nhpc_destroy_headers(nhpc_headers_t *headers) {
   
}

void nhpc_headers_add_string(nhpc_headers_t *headers, u_char *str) {

   nhpc_headers_data_t *origd = &(headers->d);

   headers->d.datasize += nhpc_strlen(str);
   
   nhpc_headers_data_t *d;
   while(headers->next)
      headers = headers->next;
   
   if(headers->d.count > headers->d.max) {
      nhpc_headers_t *n = (nhpc_headers_t *)nhpc_palloc(headers->pool, sizeof(nhpc_headers_t));

      n->d.count   = 0;
      n->d.max     = headers->d.max;
      n->d.strings = (u_char **)nhpc_palloc(headers->pool, sizeof(u_char*) * n->d.max);
      n->next      = NULL;
      
      headers->next = n;
      headers = n;
   }
   d = &headers->d;
   d->strings[d->count] = str;

   d->count++;
}

void nhpc_headers_add_new_string(nhpc_headers_t *headers, u_char *str) {

   nhpc_size_t  str_len    = nhpc_strlen(str);
   nhpc_size_t  newstr_len = nhpc_strlen(str) + 3;
   u_char      *newstr     = (u_char *)nhpc_palloc(headers->pool, newstr_len);
   memcpy(newstr, str, str_len);
   
   newstr[newstr_len - 3] = '\r';
   newstr[newstr_len - 2] = '\n';
   newstr[newstr_len - 1] = '\0';
   
   nhpc_headers_add_string(headers, newstr);
}

void nhpc_headers_add_string(nhpc_headers_t *headers, u_char *str1, u_char *str2) {

   nhpc_size_t str1_len   = nhpc_strlen(str1);
   nhpc_size_t str2_len   = nhpc_strlen(str2);
   nhpc_size_t newstr_len = str1_len + str2_len + 3;
   
   u_char *newstr = (u_char *)nhpc_palloc(headers->pool, newstr_len);
   memcpy(newstr, str1, str1_len);
   memcpy(newstr + str1_len, str2, str2_len);
   newstr[newstr_len - 3] = '\r';
   newstr[newstr_len - 2] = '\n';
   newstr[newstr_len - 1] = '\0';
   
   nhpc_headers_add_string(headers, newstr);
}

void nhpc_headers_convert_to_buffer(nhpc_headers_t *headers, nhpc_buffer_t *buffer) {

   u_char *buffer_data  = nhpc_strpalloc(buffer->pool, headers->d.datasize);
   u_char *_buffer_data = buffer_data;
   nhpc_size_t datasize = headers->d.datasize;
   
   nhpc_headers_data_t *d;
   nhpc_size_t          str_len;

   int i;
   
   while(headers) {
      d = &headers->d;

      for(i = 0; i < d->count; i++) {
	 str_len = nhpc_strlen(d->strings[i]);
	 memcpy(buffer_data, d->strings[i], str_len);
	 buffer_data += str_len;
      }

      headers = headers->next;
   }
   
   nhpc_buffer_add_header_data(buffer, _buffer_data, datasize, NHPC_BUFFER_DATA_MEM_BLOCK, 1);
}
