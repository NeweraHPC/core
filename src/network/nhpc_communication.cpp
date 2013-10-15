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

using namespace std;

void nhpc_communication_handler(nhpc_event_t *ev) {  
   nhpc_connection_t *c = (nhpc_connection_t *) ev->data;
   nhpc_status_t nrv;
   
   char  buffer[1000];
   char *partial_data;
   nhpc_size_t len = 1000;
   nhpc_size_t partial_datalen = 0;
   bzero(buffer, 1000);

   nhpc_communication_t   *communication;
   nhpc_event_handler_ptr  handler;
   
   nrv = nhpc_recv(c, buffer, &len);
   if(nrv != NHPC_SUCCESS && nrv != NHPC_EOF) {
      nhpc_log_error("ERROR: %s\n", "nhpc_recv() failed");
      return;
   }      
   
   nrv = nhpc_read_communication(c, buffer, len, &partial_data, &partial_datalen);
   if(nrv != NHPC_SUCCESS) {
      if(c->rev->eof) {
	 nhpc_accept_close_connection(c);
	 return;
      } else if(!c->communication->command_str)
	 return;
   }
   
   communication = c->communication;   
   
   if(communication->have_headers) {
      handler       = nhpc_network_search_addon(communication->command_str);
   
      if(!handler) {
	 nhpc_log_error("ERROR: %s\n", "communication_handler() failed, no suitable network addon found");
	 nhpc_accept_close_connection(c);
      } else {
	 ev->handler = handler;
	 handler(ev);
      }
   }
}

nhpc_status_t nhpc_read_communication(nhpc_connection_t *c, char *buffer, nhpc_size_t bufferlen, 
				      char **partial_data, nhpc_size_t *partial_datalen) {

   nhpc_communication_t *communication;
   if(!c->communication) {
      c->communication = (nhpc_communication_t *)nhpc_palloc(c->pool, sizeof(nhpc_communication_t));
      communication    = c->communication;

      communication->command_str  = NULL;
      communication->have_headers = 0;
      communication->data         = NULL;
      
      nhpc_rbtree_init(c->pool, &communication->headers, RBTREE_NUM);
   } else
      communication = c->communication;

   int start   = 0;
   int linelen = 0;
   int i       = 0;
   for(; i < bufferlen; i++) {
      if(buffer[i + 1] != '\0' && buffer[i] == '\r' && buffer[i + 1] == '\n') {
	 
	 linelen = i - start;

	 if(linelen == 0) {
	    if(communication->command_str) {
	       communication->have_headers = 1; 
	       i = i + 2;
	       break;	       
	    }
	    else {
	       nhpc_rbtree_insert(communication->headers, NULL);
	    }
	 } else {
	    char *linestr = (char *)nhpc_palloc(c->pool, linelen + 4);
	    nhpc_strcpy(linestr, buffer + start, linelen);

	    if(!communication->command_str)
	       communication->command_str = linestr;
	    else
	       nhpc_rbtree_insert(communication->headers, linestr);
	 }
	 
	 start = i + 2;
      }
   }
   
   if(!communication->have_headers)
      return NHPC_FAIL;
   
   if(i  < bufferlen) {
      *partial_datalen = (bufferlen - i);
      *partial_data    = buffer + i;
   }

   return NHPC_SUCCESS;
}

nhpc_status_t nhpc_analyze_stream(nhpc_connection_t *c) {
   
}

void nhpc_display_headers(nhpc_connection_t *c) {
   
}
