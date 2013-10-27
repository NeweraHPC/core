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

#include <arpa/inet.h>
#include <pthread.h>

#include <include/neweraHPC.h>

using namespace std;

nhpc_event_t      *events;
nhpc_connection_t *connections;

nhpc_status_t nhpc_init_connections(nhpc_config_t *config) {
   nhpc_str_t *connection_backlog_str = ( nhpc_str_t * )nhpc_rbtree_search(config->options, "connection_backlog");
   if( connection_backlog_str )
      connection_backlog = nhpc_atoi(connection_backlog_str->data);

   nhpc_log_debug0(LOG_LEVEL_DEBUG_4, "DEBUG: Setting connection backlog to %i\n", connection_backlog);
   
   return NHPC_SUCCESS;
}

void nhpc_init_connection(nhpc_connection_t *c) {
   
}

void nhpc_destroy_connection(nhpc_connection_t *c) {
   
}

void nhpc_shutdown_connection(nhpc_connection_t *c, int how) {

}

void nhpc_close_connection(nhpc_connection_t *c) {

}

void nhpc_init_listening(nhpc_listening_t *ls) {

}

void nhpc_destroy_listening(nhpc_listening_t *ls) {
   
}

void nhpc_get_addr_info(nhpc_connection_t *c) {
   nhpc_strcpy(c->peer.addr, inet_ntoa(c->socket.sa_in.sin_addr));
   nhpc_itoa(c->peer.port, ntohs(c->socket.sa_in.sin_port));
}