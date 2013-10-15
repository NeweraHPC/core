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

#include <sys/poll.h>
#include <errno.h>

#include <include/neweraHPC.h>

nhpc_status_t nhpc_wait_for_io_or_timeout(nhpc_socket_t *socket, int for_read) {
   pollfd pfd;
   int    rc;
   
   pfd.fd = socket->fd;
   pfd.events = for_read ? POLLIN : POLLOUT;      
   
   do { 
      rc = poll(&pfd, 1, 3000);
   } while (rc == -1 && errno == EINTR);
   
   if(rc == 0) 
      return NHPC_TIMEUP;
   else if(rc < 0) 
      return errno;
   
   return NHPC_SUCCESS;
}   

nhpc_status_t nhpc_wait_for_io_or_timeout(nhpc_connection_t *c, int for_read) {
   /*
   int rc;
      
   do {
      rc = poll(&c->readpollfd, 1, 3000);
   } while (rc == -1 && errno == EINTR);
   
   if(rc == 0) 
      return NHPC_TIMEUP;
   else if(rc < 0)
      return errno;
   */
   return NHPC_SUCCESS;
} 


