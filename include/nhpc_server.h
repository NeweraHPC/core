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

#ifndef _NHPC_SERVER_H_
#define _NHPC_SERVER_H_

#include "neweraHPC.h"

nhpc_status_t nhpc_init_network();
nhpc_status_t nhpc_create_server(const char *host_addr, const char *host_port);

typedef void (*network_addon_ptr_t)(nhpc_connection_t *c);
struct nhpc_network_handler_t
{
   network_addon_ptr_t init_request;
   network_addon_ptr_t cleanup_request;
};

extern nhpc_rbtree_t *network_addons;
#define nhpc_network_insert_addon(d, k)   (nhpc_rbtree_insert(network_addons, (void *)d, (const char *)k))
#define nhpc_network_search_addon(k)      ((nhpc_event_handler_ptr)nhpc_rbtree_search(network_addons, k))

#endif