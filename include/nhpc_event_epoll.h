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

#include <sys/epoll.h>

#define NHPC_READ_EVENT   EPOLLIN
#define NHPC_WRITE_EVENT  EPOLLOUT

#define NHPC_DELETE_EVENT   EPOLL_CTL_DEL
#define NHPC_DISABLE_EVENT  NHPC_DELETE_EVENT
#define NHPC_CLOSE_EVENT    4

nhpc_status_t nhpc_event_epoll_add_event(nhpc_event_t *ev, nhpc_int_t event, nhpc_uint_t flags);
nhpc_status_t nhpc_event_epoll_del_event(nhpc_event_t *ev, nhpc_int_t event, nhpc_uint_t flags);

nhpc_status_t nhpc_event_epoll_process_changes(nhpc_listening_t *ls);

nhpc_status_t nhpc_event_epoll_init(nhpc_listening_t *ls);
void          nhpc_event_epoll_done(nhpc_listening_t *ls);
