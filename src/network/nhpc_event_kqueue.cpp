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

#ifdef HAVE_KQUEUE

nhpc_event_actions_t nhpc_event_actions = {
   nhpc_event_kqueue_add_event,
   nhpc_event_kqueue_del_event,
   nhpc_event_kqueue_process_changes,
   nhpc_event_kqueue_init,
   nhpc_event_kqueue_done
};

int kq;
struct kevent *changelist;
struct kevent *eventlist;
int nchanges  = 0;
int nevents   = 0;
int maxevents = 0;

pthread_mutex_t mutex;

using namespace std;

nhpc_status_t nhpc_event_kqueue_set_event(nhpc_event_t *ev, nhpc_int_t filter, nhpc_uint_t flags) {
   struct kevent     *kev;  
   nhpc_connection_t *c;
   
   if(nchanges >= maxevents) {
      nhpc_log_error("ERROR:", " kqueue list full\n");
      return NHPC_FAIL;
   }
   
   kev = &changelist[nchanges];
   c   = ( nhpc_connection_t * )ev->data;
   
   kev->ident  = c->socket.fd;
   kev->filter = filter;
   kev->flags  = flags;
   kev->udata  = ev;
   
   return NHPC_SUCCESS;
}

nhpc_status_t nhpc_event_kqueue_add_event(nhpc_event_t *ev, nhpc_int_t event, nhpc_uint_t flags) {
   nhpc_status_t nrv;
   
   ev->available = 0;
   ev->eof       = 0;
   ev->close     = 0;
   ev->index     = 0;
   ev->enabled   = 1;
   
   pthread_mutex_lock(&mutex);
   
   nrv = nhpc_event_kqueue_set_event(ev, event, flags | EV_ADD | EV_ENABLE);
   ev->index = nchanges;
   nchanges++;
   
   nhpc_add_posted_event(ev);
   
   pthread_mutex_unlock(&mutex);
   
   return nrv;
}

nhpc_status_t nhpc_event_kqueue_del_event(nhpc_event_t *ev, nhpc_int_t event, nhpc_uint_t flags) {
   pthread_mutex_lock(&mutex);
   
   nhpc_event_t *e;
   
   if(ev->index > 0 && ev->index < nchanges && ev == changelist[ev->index].udata) {
      nchanges--;
      
      if(flags & NHPC_DELETE_EVENT) {
	 changelist[ev->index].flags = flags;
	 kevent(kq, &changelist[ev->index], 1, NULL, 0, NULL);
      }
      
      nhpc_del_posted_event(ev->index + 1);
      
      if(ev->index < nchanges) {
	 e = (nhpc_event_t *)changelist[nchanges].udata;
	 changelist[ev->index] = changelist[nchanges];
	 e->index = ev->index;
      }
      ev->enabled = 0;
   }   
   
   pthread_mutex_unlock(&mutex);
   return NHPC_SUCCESS;   
}

void *nhpc_event_kqueue_exec(void *data) {
   nhpc_event_t *ev = (nhpc_event_t *)data;
   
   ev->handler(ev);
   nhpc_exit_thread();
}

nhpc_status_t nhpc_event_kqueue_process_changes(nhpc_listening_t *ls) {
   /*
   int    rv;
   struct timespec timedout = {0, 100 * 1000000};
   
   nhpc_connection_t *c;
   nhpc_event_t      *ev;
   nhpc_uint_t        events;
   pthread_t          tid;
   
   pthread_mutex_lock(&mutex);
   events = kevent(kq, changelist, nchanges, eventlist, nevents, &timedout);      
   pthread_mutex_unlock(&mutex);
   
   if(events == -1) {
      cout << "errno:" << errno << endl;
      //LOG_ERROR("kevent() failed");
      return NHPC_FAIL;
   } else if(events > 0) {
      for(int i = 0; i < events; i++) {
	 
	 if(!(ev = (nhpc_event_t *)eventlist[i].udata) || ev->available) {
	    continue;
	 }
	 
	 if(!ev->accept) {
	    
	    ev->available = 1;	    
	    ev->handler(ev);
	    ev->available = 0;

	 } else {
	 
	    ev->available = eventlist[i].data;
	    ev->handler(ev);
	    ev->available = 0;

	 }
      }
   }
    */
}

nhpc_status_t nhpc_event_kqueue_init() {
   if( (kq = kqueue()) == -1 )
      return errno;
   
   maxevents  = 512;
   nevents    = maxevents;
   nchanges   = 0;
   changelist = ( struct kevent * )nhpc_calloc(sizeof(struct kevent) * maxevents);
   eventlist  = ( struct kevent * )nhpc_calloc(sizeof(struct kevent) * maxevents);
   
   if(!changelist) 
      return NHPC_FAIL;
   
   pthread_mutex_init(&mutex, NULL);
   
   return NHPC_SUCCESS;

   /*
   nhpc_event_t *ev = &ls->events[0];
   ev->accept       = 1;
   ev->active       = 1;
   ev->handler      = nhpc_accept_event;
   ev->data         = ls;
   ev->enabled      = 1;
   
   struct kevent *kev = changelist;
   kev->ident     = ls->socket.fd;
   kev->filter    = EVFILT_READ;
   kev->flags     = EV_ADD | EV_ENABLE;
   kev->udata     = ev;
   nchanges++;
   nhpc_add_posted_event(ev);
    */
}

nhpc_status_t nhpc_event_kqueue_done(nhpc_listening_t *ls) {
   
}

#endif
