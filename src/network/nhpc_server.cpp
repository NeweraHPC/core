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

nhpc_status_t nhpc_create_listening_server(nhpc_listening_t **listener, nhpc_config_t *config, 
					   const char *host_addr, const char *host_port) {
   nhpc_status_t     nrv;
   nhpc_listening_t *_listener;
   
   nhpc_int_t enable_opts = 1;
   
   _listener = ( nhpc_listening_t * )nhpc_calloc(sizeof( nhpc_listening_t ));
   if( (nrv = nhpc_create_socket(&_listener->socket, AF_INET, SOCK_STREAM, 0)) != NHPC_SUCCESS ) {
      nhpc_log_error("ERROR: unable to create socket: %s\n", strerror(errno));
      return nrv;
   }
   
   if( (nrv = nhpc_set_socket_opt(&_listener->socket, NHPC_NONBLOCK, 1)) != NHPC_SUCCESS ) {
      nhpc_log_error("ERROR: unable to set socket options: %s\n", strerror(errno));
      return nrv;
   }
   
   if( (nrv = setsockopt(_listener->socket.fd, SOL_SOCKET, SO_REUSEADDR, &enable_opts, sizeof(int))) == -1 ) {
      nhpc_log_error("ERROR: unable to set socket options: %s\n", strerror(errno));
      return errno;
   }      
   
   if( (nrv = nhpc_bind_socket(&_listener->socket, host_addr, host_port)) != NHPC_SUCCESS ) {
      nhpc_log_error("ERROR: unable to bind socket: %s\n", strerror(errno));
      return nrv;
   }
   
   if( (nrv = nhpc_listen_socket(&_listener->socket, connection_backlog)) != NHPC_SUCCESS) {
      nhpc_log_error("ERROR: unable to listen socket: %s\n", strerror(errno));
      return nrv;
   }
   
   *listener = _listener;
   return NHPC_SUCCESS;
}

nhpc_status_t nhpc_register_listening_server(nhpc_listening_t *listener) {
   nhpc_add_event(&listener->lsev, NHPC_READ_EVENT, 0);
}

void *nhpc_handle_connection(nhpc_connection_t *c) {
   nhpc_exit_thread();
   return NULL;
}