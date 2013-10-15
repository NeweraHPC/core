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

nhpc_status_t nhpc_send(nhpc_connection_t *c, char *buffer, nhpc_size_t *len) {
   if(c->wev->eof)
      return NHPC_EOF;
   
   int   rv;
   char *tmp_buffer = buffer;
   nhpc_size_t tmp_len = *len;
   
   do {
      rv = write(c->socket.fd, tmp_buffer, tmp_len);
      if(rv > 0) {
	 tmp_len    -= rv;
	 tmp_buffer += rv;
      }
   } while(rv == -1 && errno == EINTR && tmp_len > 0);
   
   *len = *len - tmp_len;
   
   if(rv == 0) {
      c->wev->eof = 1;

      return NHPC_EOF;
   }
   
   if(rv == -1)
      return errno;
   
   return NHPC_SUCCESS;
}

nhpc_status_t nhpc_send_buffer(nhpc_connection_t *c, nhpc_buffer_t *buffer) {
   
   if(!buffer->head)
      return NHPC_EOF;
   
   nhpc_size_t size;
   nhpc_status_t nrv;
   
   u_char  send_buffer[MAX_BUFFER_SIZE];
   u_char *tmp_send_buffer = send_buffer;
   
   u_char *tmp_start;
   
   nhpc_size_t send_buffer_len = 0;
   nhpc_size_t tmp_send_buffer_len;
   
   do {
     
      size      = buffer->head->end - buffer->head->start;
      tmp_start = buffer->head->start;
      
      if(size <= MAX_BUFFER_SIZE) {
	 
	 tmp_send_buffer_len = size;
	 buffer->head = buffer->head->next;
	 
      } else {
	 
	 tmp_send_buffer_len = MAX_BUFFER_SIZE;
	 
      }
      
      memcpy(tmp_send_buffer, tmp_start, tmp_send_buffer_len);
      send_buffer_len += tmp_send_buffer_len;
      tmp_send_buffer += tmp_send_buffer_len;
      
   } while(send_buffer_len < MAX_BUFFER_SIZE && buffer->head);
   
   nrv = nhpc_send(c, (char *)send_buffer, &send_buffer_len);
   if(nrv != NHPC_SUCCESS) {
      nhpc_log_error("ERROR: send() failed, errno:%i\n", nrv);
      return nrv;
   }

   return nrv;
}

nhpc_status_t nhpc_send_file(nhpc_connection_t *c, char *filepath) {
   
}

void nhpc_send_file_handler(nhpc_event_t *ev) {
   
}
