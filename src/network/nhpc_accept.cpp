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

void nhpc_accept_event(nhpc_event_t *ev) {
   /*
   nhpc_listening_t  *ls = (nhpc_listening_t *)ev->data;
   nhpc_connection_t *c;

   int       new_sockfd;
   pthread_t tid;
   
   do {
      c = (nhpc_connection_t *)nhpc_pop_stack(ls->connections_stack);
      if(!c->pool) {
	 nhpc_log_debug0(LOG_LEVEL_DEBUG_4, "DEBUG: %s\n", "allocating new c->pool");
	 c->pool = nhpc_create_pool(1);
      }
      c->communication = NULL;
      
      new_sockfd = accept(ls->socket.fd, (struct sockaddr *)&c->socket.sa_in, &c->socket.socklen);
      if(new_sockfd == -1) {
	 nhpc_accept_free_connection(c);
	 break;
      }
      c->socket.fd = new_sockfd;
      
      c->rev->active = 1;
      c->rev->index  = -1;
      c->wev->active = 1;
      c->wev->index  = -1;      
      
      c->rev->handler = nhpc_communication_handler;
      nhpc_add_event(c->rev, NHPC_READ_EVENT, 0);
      
#ifdef HAVE_KQUEUE
      ev->available--;
#endif
      
   } while (ev->available > 0);
    */
}

