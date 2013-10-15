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

void nhpc_init_connection(nhpc_connection_t *c) {
   
}

void nhpc_destroy_connection(nhpc_connection_t *c) {
   
}

void nhpc_shutdown_connection(nhpc_connection_t *c, int how) {

   if(how == SHUT_RD || how == SHUT_RDWR) {
      if(c->rev->enabled)
	 nhpc_del_event(c->rev, NHPC_READ_EVENT, NHPC_DELETE_EVENT);
      
      shutdown(c->socket.fd, SHUT_RD);      
   }
   
   if(how == SHUT_WR || how == SHUT_RDWR) {      
      if(c->wev->enabled)
	 nhpc_del_event(c->wev, NHPC_WRITE_EVENT, NHPC_DELETE_EVENT);
      
      shutdown(c->socket.fd, SHUT_WR);
   }
}

void nhpc_close_connection(nhpc_connection_t *c) {
   if(c->rev->enabled)
      nhpc_del_event(c->rev, NHPC_READ_EVENT, NHPC_CLOSE_EVENT);
   if(c->wev->enabled)
      nhpc_del_event(c->wev, NHPC_WRITE_EVENT, NHPC_CLOSE_EVENT);

   close(c->socket.fd);
}

void nhpc_init_listening(nhpc_listening_t *ls) {
   ls->nconnections = CONNECTION_BACKLOG;
   ls->connections  = (nhpc_connection_t *)nhpc_calloc(sizeof(nhpc_connection_t) * ls->nconnections);
   ls->events       = (nhpc_event_t *)nhpc_calloc(sizeof(nhpc_event_t) * (ls->nconnections * 2 + 1));

   //ls->connections_queue = nhpc_init_queue(CONNECTION_BACKLOG);
   
   ls->pool = nhpc_create_pool(1);
   ls->connections_stack = nhpc_init_stack(CONNECTION_BACKLOG, ls->pool);
   
   nhpc_event_t *rev, *wev;
   
   for(int i = 0; i < CONNECTION_BACKLOG; i++) {
      rev = &ls->events[2 * i + 1];
      wev = &ls->events[2 * i + 2];
      
      rev->data = &ls->connections[i];
      rev->instance = 1;

      wev->data = &ls->connections[i];
      wev->instance = 1;
      wev->write    = 1;
      
      ls->connections[i].rev = rev;
      ls->connections[i].wev = wev;      
      ls->connections[i].ls  = ls;

      nhpc_push_stack(ls->connections_stack, &ls->connections[i]);
      //nhpc_insert_queue(ls->connections_queue, &ls->connections[i]);
   }
}

void nhpc_destroy_listening(nhpc_listening_t *ls) {
   
}

void nhpc_get_addr_info(nhpc_connection_t *c) {
   nhpc_strcpy(c->peer.addr, inet_ntoa(c->socket.sa_in.sin_addr));
   nhpc_itoa(c->peer.port, ntohs(c->socket.sa_in.sin_port));
}

