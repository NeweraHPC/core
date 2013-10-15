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

#ifndef _NHPC_ATOMIC_H_
#define _NHPC_ATOMIC_H_

#include <stdint.h>
#include <cstddef>

#define NHPC_SUCCESS  0
#define NHPC_FAIL    -1

#ifdef __x86_64__
typedef uint64_t nhpc_uint_t;
typedef int64_t  nhpc_int_t;
#else
typedef uint32_t nhpc_uint_t;
typedef int32_t  nhpc_int_t;
#endif

typedef nhpc_int_t  nhpc_status_t;
typedef size_t      nhpc_size_t;

typedef unsigned char u_char;

#endif