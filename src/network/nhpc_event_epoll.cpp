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
#include <stdio.h>

#include <include/neweraHPC.h>

#ifdef HAVE_EPOLL

int efd = 0;
int maxevents = 0;
int nchanges  = 0;
struct epoll_event *epoll_events;

pthread_mutex_t mutex;

nhpc_event_actions_t nhpc_event_actions = {
   nhpc_event_epoll_add_event,
   nhpc_event_epoll_del_event,
   nhpc_event_epoll_process_changes,
   nhpc_event_epoll_init,
   nhpc_event_epoll_done,
};

nhpc_status_t nhpc_event_epoll_set_event(nhpc_event_t *ev, nhpc_int_t event, nhpc_uint_t flags) {
 
   int rv;
   struct epoll_event epoll_event;
   nhpc_connection_t *c = (nhpc_connection_t *)ev->data;

   epoll_event.data.ptr = ev; 
   epoll_event.events   = event;

   if((rv = epoll_ctl(efd, flags, c->socket.fd, &epoll_event)) == -1) {
      nhpc_log_error("ERROR: epoll_ctl failed for socket:%i\n", c->socket.fd);
      perror("epoll_ctl");
      pthread_mutex_unlock(&mutex);
    
      return errno;
   }

   return NHPC_SUCCESS;
}

nhpc_status_t nhpc_event_epoll_add_event(nhpc_event_t *ev, nhpc_int_t event, nhpc_uint_t flags) {

   int rv;
   nhpc_status_t nrv;

   ev->available = 0;
   ev->eof       = 0;
   ev->close     = 0;
   ev->index     = 0;
   ev->enabled   = 1;
   
   pthread_mutex_lock(&mutex);

   if((nrv = nhpc_event_epoll_set_event(ev, event, flags | EPOLL_CTL_ADD)) != NHPC_SUCCESS) {
      pthread_mutex_unlock(&mutex);
      return nrv; 
   }

   nhpc_add_posted_event(ev);
   ev->index = nchanges;
   nchanges++;

   pthread_mutex_unlock(&mutex);

   return NHPC_SUCCESS;
}

nhpc_status_t nhpc_event_epoll_del_event(nhpc_event_t *ev, nhpc_int_t event, nhpc_uint_t flags) {
   
   struct epoll_event epoll_event;   
   nhpc_status_t nrv;

   ev->enabled = 0;
 
   pthread_mutex_lock(&mutex);

   nhpc_del_posted_event(ev->index + 1);
   nchanges--;

   if(flags & NHPC_CLOSE_EVENT) {
      pthread_mutex_unlock(&mutex);
      return NHPC_SUCCESS;
   }

   if(flags & NHPC_CLOSE_EVENT) {
      pthread_mutex_unlock(&mutex);
      return NHPC_SUCCESS;
   } else if((nrv = nhpc_event_epoll_set_event(ev, event, flags | EPOLL_CTL_DEL)) != NHPC_SUCCESS) {
      pthread_mutex_unlock(&mutex);
      return nrv;
   }
   pthread_mutex_unlock(&mutex);

   return NHPC_SUCCESS;
}

nhpc_status_t nhpc_event_epoll_process_changes(nhpc_listening_t *ls) {
   
   int rv;
   int events;

   nhpc_event_t *ev;

   pthread_mutex_lock(&mutex);
   events = epoll_wait(efd, epoll_events, maxevents, 100);
   pthread_mutex_unlock(&mutex);
   nhpc_log_debug1(LOG_LEVEL_DEBUG_3, "DEBUG: %s:%i\n", "epoll_wait() pending events", events);

   if(events == -1) {

      nhpc_log_error("ERROR: %s\n", "epoll_wait() failed");
      return errno;

   } else if(events > 0) {

      for(int i = 0; i < events; i++) {

         if(epoll_events[i].events & EPOLLERR || epoll_events[i].events & EPOLLHUP) {
            nhpc_log_debug1(LOG_LEVEL_DEBUG_3, "DEBUG: %s, closing socket fd:%i\n", "epoll_event error or closed", epoll_events[i].data.fd);
            continue;
         }

         if(!(ev = (nhpc_event_t *)epoll_events[i].data.ptr)) {
            nhpc_log_error("ERROR: %s\n", "epoll_event data not found");
            return NHPC_FAIL;
         }

         if(!ev->accept) {
	    ev->handler(ev);
            ev->available = 0;
         } else {
   	    ev->handler(ev);
	    ev->available = 0;
         }
      }
   }

   return NHPC_SUCCESS;
}

nhpc_status_t nhpc_event_epoll_init(nhpc_listening_t *ls) {

   int rv;
   struct epoll_event event;

   maxevents = ls->nconnections + 1;

   if((efd = epoll_create(maxevents)) == -1) {
      nhpc_log_error("ERROR: %s\n", "epoll_create() failed");
      return errno;
   }

   nhpc_event_t *ev = &ls->events[0];
   ev->accept       = 1;
   ev->active       = 1;
   ev->handler      = nhpc_accept_event;
   ev->data         = ls;
   ev->enabled      = 1;
   
   event.data.ptr = ev;
   event.events   = EPOLLIN | EPOLLET;

   nhpc_add_posted_event(ev);

   rv = epoll_ctl(efd, EPOLL_CTL_ADD, ls->socket.fd, &event);
   if(rv == -1) {
      nhpc_log_error("ERROR: %s\n", "epoll_ctl() failed");
      return errno;
   }
   nchanges++;
   
   epoll_events = (struct epoll_event *)nhpc_pcalloc(ls->pool, maxevents * sizeof(struct epoll_event));

   pthread_mutex_init(&mutex, NULL);

   return NHPC_SUCCESS;
}

void nhpc_event_epoll_done(nhpc_listening_s *ls) {

}

#endif
