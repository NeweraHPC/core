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

#ifndef _NHPC_EVENT_H_
#define _NHPC_EVENT_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#ifdef HAVE_KQUEUE
#include "nhpc_event_kqueue.h"
#elif  HAVE_EPOLL
#include "nhpc_event_epoll.h"
#else
#include <poll.h>
#include "nhpc_event_poll.h"
#endif
#endif

#ifndef NHPC_CLOSE_EVENT
#define NHPC_CLOSE_EVENT 1
#endif
#ifndef NHPC_DISABLE_EVENT 
#define NHPC_DISABLE_EVENT 2
#endif
#ifndef NHPC_DELETE_EVENT
#define NHPC_DELETE_EVENT 4
#endif

#ifndef NHPC_EOF
#define NHPC_EOF 0x8000
#endif

struct nhpc_event_actions_t {
   nhpc_status_t  (*add)(nhpc_event_t *ev, nhpc_int_t event, nhpc_uint_t flags);
   nhpc_status_t  (*del)(nhpc_event_t *ev, nhpc_int_t event, nhpc_uint_t flags);
            
   nhpc_status_t  (*process_changes)(nhpc_listening_t *listener);
      
   nhpc_status_t  (*init)();
   nhpc_status_t  (*done)(nhpc_listening_t *listener);
};
extern nhpc_event_actions_t nhpc_event_actions;

#define nhpc_add_event        nhpc_event_actions.add
#define nhpc_del_event        nhpc_event_actions.del
#define nhpc_process_changes  nhpc_event_actions.process_changes
#define nhpc_done_event       nhpc_event_actions.done
nhpc_status_t nhpc_init_event();

struct nhpc_event_s {
   void                  *data;
   nhpc_status_t          status;
   
   nhpc_int_t             index;     
   
   unsigned               write:1;
   unsigned               accept:1;
   
   unsigned               active:1;
   unsigned               enabled:1;

   unsigned               ready:1;
#ifdef HAVE_KQUEUE
   nhpc_uint_t            available;
   nhpc_uint_t            kq_error;   
#else
   unsigned               available:1;
#endif
   
   unsigned               eof;
   
   unsigned               close;
   
   unsigned               posted_enabled;
   
   nhpc_event_handler_ptr handler;   
};
extern nhpc_rbtree_t *nhpc_posted_events;
#define nhpc_add_posted_event(ev)         (nhpc_rbtree_insert(nhpc_posted_events, ev))
#define nhpc_del_posted_event(i)          (nhpc_rbtree_erase(nhpc_posted_events, i))         
#define nhpc_get_posted_event(i)          ((nhpc_event_t *)nhpc_rbtree_search(nhpc_posted_events, i))
#define nhpc_get_posted_event_length()    (nhpc_rbtree_length(nhpc_posted_events))

void *nhpc_monitor_posted_events(nhpc_listening_t *ls);
void  nhpc_init_posted_events(nhpc_listening_t *ls);

#endif
