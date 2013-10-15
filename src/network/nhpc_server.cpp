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

#include <include/neweraHPC.h>

using namespace std;

nhpc_listening_t  *ls;
nhpc_rbtree_t     *network_addons;

nhpc_status_t nhpc_init_network() {
   nhpc_init_threads(1024, 16 * 1024);
   nhpc_rbtree_init(main_pool, &network_addons, RBTREE_STR);
}

nhpc_status_t nhpc_create_server(const char *host_addr, const char *host_port)
{
   nhpc_status_t nrv;
   pthread_t read_event;
   pthread_t write_event;
   
   ls = (nhpc_listening_t *)nhpc_alloc(sizeof(nhpc_listening_t));
   
   if((nrv = nhpc_create_socket(&ls->socket, AF_INET, SOCK_STREAM, 0)) != NHPC_SUCCESS)
   {
      nhpc_log_error("ERROR: %s\n", "socket() failed");
      return nrv;
   }
   
   if((nrv = nhpc_set_socket_opt(&ls->socket, NHPC_NONBLOCK, 1)) != NHPC_SUCCESS)
   {
      nhpc_log_error("ERROR %s\n", "socket_opt() failed");
      return nrv;
   }
   
   int enable_opts = 1;
   if((nrv = setsockopt(ls->socket.fd, SOL_SOCKET, SO_REUSEADDR, &enable_opts, sizeof(int))) == -1)
   {
      return errno;
   }   
   
   if((nrv = nhpc_bind_socket(&ls->socket, host_addr, host_port)) != NHPC_SUCCESS)
   {
      nhpc_log_error("ERROR: %s\n", "socket_bind() failed");
      return nrv;
   }
   
   if((nrv = nhpc_listen_socket(&ls->socket, CONNECTION_BACKLOG)) != NHPC_SUCCESS)
   {
      nhpc_log_error("ERROR: %s\n", "socket_listen() failed");
      return nrv;
   }
   
   nhpc_init_listening(ls);
   nhpc_init_event(ls);
   //nhpc_init_worker_pool(main_pool, 20);
   nhpc_init_posted_events(ls);
   
   return NHPC_SUCCESS;
}

void *nhpc_handle_connection(nhpc_connection_t *c) {
   nhpc_exit_thread();
   return NULL;
}