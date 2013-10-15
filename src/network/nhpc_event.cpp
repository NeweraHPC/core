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

nhpc_rbtree_t   *nhpc_posted_events;
pthread_mutex_t  posted_event_mutex;

nhpc_status_t nhpc_init_event(nhpc_listening_t *ls) {
   nhpc_rbtree_init(ls->pool, &nhpc_posted_events, RBTREE_NUM_MANAGED);

   nhpc_event_actions.init(ls);
}

void *nhpc_monitor_posted_events(nhpc_listening_t *ls) {

   bool exec_event = false;
   int event_count;
   nhpc_event_t *ev;
   
   for(;;) {
      event_count = nhpc_get_posted_event_length();
      if(!event_count) {
	 cout << "No event to post" << endl;
	 continue;
      }
      
      exec_event = false;
      
      nhpc_socket_t  *s;
      
      for(int i = 1; i <= event_count; i++) {
	 ev = nhpc_get_posted_event(i);
	 
	 s = (nhpc_socket_t *) ev->data;
	 
	 if(!ev->available && ev->enabled) {
	    nhpc_process_changes(ls);
	    break;
	 }
      }
   }   
}

void nhpc_init_posted_events(nhpc_listening_t *ls) {
   pthread_t tid;
   
   nhpc_create_thread(&tid, (void* (*)(void*))nhpc_monitor_posted_events, ls);
}