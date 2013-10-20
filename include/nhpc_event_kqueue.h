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

#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>

#define NHPC_READ_EVENT   EVFILT_READ
#define NHPC_WRITE_EVENT  EVFILT_WRITE

#define NHPC_CLOSE_EVENT   EV_EOF
#define NHPC_DISABLE_EVENT EV_DISABLE
#define NHPC_DELETE_EVENT  EV_DELETE

#define NHPC_EOF EV_EOF

nhpc_status_t nhpc_event_kqueue_add_event(nhpc_event_t *ev, nhpc_int_t event, nhpc_uint_t flags);
nhpc_status_t nhpc_event_kqueue_del_event(nhpc_event_t *ev, nhpc_int_t event, nhpc_uint_t flags);

nhpc_status_t nhpc_event_kqueue_process_changes();

nhpc_status_t nhpc_event_kqueue_init();
void          nhpc_event_kqueue_done();