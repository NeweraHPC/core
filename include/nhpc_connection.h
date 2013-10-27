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

#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include <poll.h>

#include "neweraHPC.h"

//#define CONNECTION_BACKLOG 551
static int connection_backlog = 551;
#define BUFFER_SIZE        10000

#define NHPC_BUFFER_EMPTY  0
#define NHPC_BUFFER_FULL   1
#define NHPC_BUFFER_LOCKED 2

#define NHPC_CONNECTION_OCCUPIED       1
#define NHPC_CONNECTION_READING        2
#define NHPC_CONNECTION_WRITING        4
#define NHPC_CONNECTION_ENABLE_READ    8
#define NHPC_CONNECTION_ENABLE_WRITE  16
#define NHPC_CONNECTION_CLOSE         32

struct nhpc_peer_addr_t
{
   char addr[16];
   char port[6];
};

struct nhpc_connection_s {
   nhpc_socket_t           socket;
   nhpc_peer_addr_t        peer;
   
   nhpc_listening_t       *ls;
   nhpc_event_t            rev;
   nhpc_event_t            wev;
   
   nhpc_pool_t            *pool;
   
   nhpc_communication_t   *communication;
};

struct nhpc_listening_s {
   nhpc_socket_t       socket;
   nhpc_peer_addr_t    host;   

   nhpc_pool_t        *pool;
      
   nhpc_stack_t       *connections_stack;
   nhpc_int_t          nconnections;
   
   nhpc_event_t        lsev;
   
   nhpc_rbtree_t      *network_addons;

   int                 backlog;
};
#define nhpc_lock_listening_connection(ls, m)    (nhpc_mutex_lock(&(ls)->mutex, m))
#define nhpc_unlock_listening_connection(ls, m)  (nhpc_mutex_unlock(&(ls)->mutex, m))

void nhpc_get_addr_info(nhpc_connection_t *c);

void nhpc_init_listening(nhpc_listening_t *ls);
void nhpc_destroy_listeneing(nhpc_listening_t *ls);

nhpc_status_t nhpc_init_connections(nhpc_config_t *config);

void nhpc_init_connection(nhpc_connection_t *c);
void nhpc_destroy_connection(nhpc_connection_t *c);
void nhpc_shutdown_connection(nhpc_connection_t *c, int how);
void nhpc_close_connection(nhpc_connection_t *c);

#endif