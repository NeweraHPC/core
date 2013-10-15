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

#ifndef _NETWORK_H_
#define _NETWORK_H_

typedef struct nhpc_event_s          nhpc_event_t;
typedef struct nhpc_connection_s     nhpc_connection_t;
typedef struct nhpc_listening_s      nhpc_listening_t;
typedef struct nhpc_communication_s  nhpc_communication_t;
typedef struct nhpc_headers_s        nhpc_headers_t;
typedef struct nhpc_worker_s	     nhpc_worker_t;
typedef struct nhpc_worker_pool_s    nhpc_worker_pool_t;

typedef struct nhpc_http_request_s   nhpc_http_request_t;
typedef struct nhpc_http_status_s    nhpc_http_status_t;

typedef void (*nhpc_event_handler_ptr)(nhpc_event_t *ev);
typedef void (*nhpc_connection_handler_ptr)(nhpc_connection_t *c);

#include "nhpc_headers.h"
#include "nhpc_socket.h"
#include "nhpc_connection.h"
#include "nhpc_server.h"
#include "nhpc_waitio.h"
#include "nhpc_http.h"
#include "nhpc_communication.h"
#include "nhpc_send.h"
#include "nhpc_recv.h"
#include "nhpc_event.h"
#include "nhpc_worker.h"
#include "nhpc_accept.h"

#endif