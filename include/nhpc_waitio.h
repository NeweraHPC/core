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

#ifndef _NHPC_WAITIO_H_
#define _NHPC_WAITIO_H_

#include <sys/errno.h>

#define NHPC_TIMEUP  ETIMEDOUT

nhpc_status_t nhpc_wait_for_io_or_timeout(nhpc_socket_t *sock, int for_read);
nhpc_status_t nhpc_wait_for_io_or_timeout(nhpc_connection_t *c, int for_read);

#endif