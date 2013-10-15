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

#ifndef _NHPC_COMMUNICATION_H_
#define _NHPC_COMMUNICATION_H_

struct nhpc_communication_s {
   char                   *command_str;
   nhpc_rbtree_t          *headers;

   unsigned                have_headers:1;
   
   void                   *data;
   
   nhpc_event_handler_ptr  handler;
};

void          nhpc_communication_handler(nhpc_event_t *ev);
nhpc_status_t nhpc_read_communication(nhpc_connection_t *c, char *buffer, nhpc_size_t bufferlen, 
				      char **partial_data, nhpc_size_t *partial_datalen);

#endif