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

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include <include/neweraHPC.h>

nhpc_status_t nhpc_create_socket(nhpc_socket_t *s, int family, int type, int protocol) {
   if((s->fd = socket(family, type, protocol)) < 0)
      return errno;
   
   s->family   = family;
   s->type     = type;
   s->protocol = protocol;
   
   return NHPC_SUCCESS;
}

nhpc_status_t nhpc_bind_socket(nhpc_socket_t *socket, const char *host_addr, const char *host_port)
{
   struct sockaddr_in sa;
   
   sa.sin_family = AF_INET; 
   sa.sin_port   = htons(atoi(host_port)); 
   inet_aton(host_addr, &sa.sin_addr);
   
   if(bind(socket->fd, (struct sockaddr *)&sa, sizeof(sockaddr)) < 0)
      return errno;
   
   return NHPC_SUCCESS;
}

nhpc_status_t nhpc_listen_socket(nhpc_socket_t *socket, int connection_queue)
{
   if(listen(socket->fd, connection_queue) < 0)
      return errno;
   
   return NHPC_SUCCESS;
}

#define socket_set_opt(skt, option, on)	   \
do {                                       \
if (on)                                    \
(skt)->socketopt |= (option);              \
else                                       \
(skt)->socketopt &= ~(option);             \
} while (0)

nhpc_status_t nhpc_set_socket_opt(nhpc_socket_t *socket, nhpc_socket_opt_t sockopt, int on)
{
   int one;
   nhpc_status_t status;
   
   one = on ? 1 : 0;
   
   switch (sockopt)
   {
      case NHPC_REUSEADDR:
	 if (on != nhpc_socket_is_opt_set(socket, sockopt))
	 {
	    status = setsockopt(socket->fd, SOL_SOCKET, NHPC_REUSEADDR, 
				(void *)&one, sizeof(int));
	    if (status == -1)
	       return errno;
	 }
	 break;
      case NHPC_NONBLOCK:
	 if (nhpc_socket_is_opt_set(socket, NHPC_NONBLOCK) != on) 
	 {
	    if (on) 
	    {
	       if ((status = nhpc_set_socket_nonblock(socket)) != NHPC_SUCCESS) 
		  return status;
	    }
	    else 
	    {
	       if ((status = nhpc_set_socket_block(socket)) != NHPC_SUCCESS)
		  return status;
	    }
	    socket_set_opt(socket, NHPC_NONBLOCK, on);
	 }
	 break;	    
   }
   
   return NHPC_SUCCESS;
}

nhpc_status_t nhpc_set_socket_nonblock(nhpc_socket_t *socket)
{
   int rv = fcntl(socket->fd, F_SETFL, O_NONBLOCK);
   if(rv == -1)return errno;
   
   return NHPC_SUCCESS;
}

nhpc_status_t nhpc_set_socket_block(nhpc_socket_t *socket)
{
   int fd_flags = fcntl(socket->fd, F_GETFL, 0);
   fd_flags &= ~O_NONBLOCK;
   
   int rv = fcntl(socket->fd, F_SETFL, fd_flags);
   if(rv == -1)return errno;
   
   return NHPC_SUCCESS;
}