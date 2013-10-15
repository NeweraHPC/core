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

#ifndef _NHPC_THREAD_H_
#define _NHPC_THREAD_H_

#include <pthread.h>

#define NHPC_THREAD_LOCK_READ  1
#define NHPC_THREAD_LOCK_WRITE 0

struct nhpc_mutex_t {
   pthread_rwlock_t lock;
};
nhpc_status_t nhpc_mutex_init(nhpc_mutex_t *mutex);
nhpc_status_t nhpc_mutex_lock(nhpc_mutex_t *mutex, int for_read);
nhpc_status_t nhpc_mutex_trylock(nhpc_mutex_t *mutex, int for_read);
nhpc_status_t nhpc_mutex_unlock(nhpc_mutex_t *mutex, int for_read);
nhpc_status_t nhpc_mutex_destroy(nhpc_mutex_t *mutex);   

typedef void *nhpc_thread_value_t(void *arg);

nhpc_status_t nhpc_init_threads(int n, nhpc_size_t);
nhpc_status_t nhpc_create_thread(pthread_t *tid, nhpc_thread_value_t func, void *arg);
void          nhpc_exit_thread();
#define nhpc_thread_join(t, p) pthread_join(t, p)


#endif