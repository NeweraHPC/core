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

#include <time.h>
#include <sys/time.h>

#include <include/neweraHPC.h>

using namespace std;

struct timeval tv;
struct timespec ts;

nhpc_status_t nhpc_recv(nhpc_connection_t *c, char *buffer, nhpc_size_t *len) {
   if(c->rev->eof)
      return NHPC_EOF;
   
   int   rv;
   char *tmp_buffer = buffer;
   nhpc_size_t tmp_len = *len;
   
   do {
      rv = read(c->socket.fd, tmp_buffer, tmp_len);
      if(rv > 0) {
	 tmp_len    -= rv;
	 tmp_buffer += rv;
      } 
   } while(rv == -1 && errno == EINTR && tmp_len > 0);

   *len = (*len - tmp_len);
   
   if(rv == 0) {
      c->rev->eof = 1;
      
      return NHPC_EOF;
   } else if(rv == -1) {
   }
   
   return NHPC_SUCCESS;
}
