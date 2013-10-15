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

#ifndef _NHPC_HTTP_REQUEST_H_
#define _NHPC_HTTP_REQUEST_H_

#define HTTP_ROOT "/www"
extern nhpc_size_t http_root_strlen;

typedef void (*nhpc_http_handler_ptr)(nhpc_http_request_t *);

#define NHPC_HTTP_GET_REQUEST     0x0001
#define NHPC_HTTP_POST_REQUEST    0x0002
#define NHPC_HTTP_INVALID_REQUEST 0x0004

#define NHPC_HTTP_VERSION_1_0     0x0001
#define NHPC_HTTP_VERSION_1_1	  0x0002

struct nhpc_http_request_types {
   const char  *str;
   nhpc_uint_t  value;
};
extern nhpc_http_request_types http_request_types[];

struct nhpc_http_status_s {
   unsigned         version;
   nhpc_uint_t      request_type;

   nhpc_uint_t      status_code;
   const u_char    *status_str;

   nhpc_headers_t  *headers;
};

struct nhpc_http_request_s {
   nhpc_connection_t       *c;
   nhpc_communication_t    *communication;
   nhpc_pool_t             *pool;
   
   nhpc_http_handler_ptr    read_handler;
   nhpc_http_handler_ptr    write_handler;
   
   nhpc_http_status_s       status;
   
   char                    *request_str;
   nhpc_rbtree_t           *request_get_variables;
   
   unsigned                 read:1;
   unsigned                 write:1;
   
   nhpc_buffer_t           *response_buffer;
};

void nhpc_http_handler(nhpc_event_t *ev);
void nhpc_http_write_handler(nhpc_event_t *ev);

nhpc_http_request_t *nhpc_http_init_request_data(nhpc_pool_t *p, nhpc_communication_t *cm);

#define NHPC_HTTP_STATUS_OK                    200

#define NHPC_HTTP_STATUS_MOVED_PERMANENTLY     301
#define NHPC_HTTP_STATUS_FOUND                 302

#define NHPC_HTTP_STATUS_BAD_REQUEST           400
#define NHPC_HTTP_STATUS_UNAUTHRIZED           401
#define NHPC_HTTP_STATUS_FORBIDDEN             403
#define NHPC_HTTP_STATUS_NOT_FOUND             404

#define NHPC_HTTP_STATUS_INTERNAL_SERVER_ERROR 500
#define NHPC_HTTP_STATUS_NOT_IMPLEMENTED       501

#endif