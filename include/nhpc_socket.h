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

#ifndef _NHPC_SOCKET_H_
#define _NHPC_SOCKET_H_

#include <sys/socket.h>
#include <netinet/in.h>

#include "neweraHPC.h"

#define NHPC_NONBLOCK  1
#define NHPC_REUSEADDR 2

typedef unsigned int  nhpc_fd_t;
typedef u_char        nhpc_socket_opt_t;

struct nhpc_socket_t {
   struct sockaddr_in  sa_in;
   socklen_t           socklen;
   
   int		       family;
   int		       type;
   int		       protocol;
   
   nhpc_fd_t           fd;
   nhpc_socket_opt_t   socketopt;  
};

nhpc_status_t nhpc_create_socket(nhpc_socket_t *socket, int family, int type, int protocol);
nhpc_status_t nhpc_bind_socket(nhpc_socket_t *socket, const char *host_addr, const char *host_port);
nhpc_status_t nhpc_listen_socket(nhpc_socket_t *socket, int connection_queue);

nhpc_status_t nhpc_set_socket_opt(nhpc_socket_t *socket, nhpc_socket_opt_t socketopt, int on);
nhpc_status_t nhpc_set_socket_nonblock(nhpc_socket_t *sock);
nhpc_status_t nhpc_set_socket_block(nhpc_socket_t *sock);

#define nhpc_socket_is_opt_set(skt, option)  (((skt)->socketopt & (option)) == (option))

#endif