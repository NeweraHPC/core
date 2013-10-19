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

#ifndef _LOG_H_
#define _LOG_H_

#include <iostream>

extern nhpc_uint_t log_level;

#define LOG_LEVEL_DEBUG_0  1
#define LOG_LEVEL_DEBUG_1  2
#define LOG_LEVEL_DEBUG_2  4
#define LOG_LEVEL_DEBUG_3  8
#define LOG_LEVEL_DEBUG_4 16
#define LOG_LEVEL_DEBUG_5 32

#define LOG_LEVEL_DEBUG_MAX LOG_LEVEL_DEBUG_5

#define NHPC_LOG_DEBUG_PREFIX
#define NHPC_LOG_ERROR_PREFIX

void nhpc_log_init(const u_char *log_str);
void nhpc_log_debug(const char *fmt, ...);

#define _nhpc_log_debug(level, fmt, arg1, arg2, arg3, arg4, arg5, arg6) \
   if(log_level & level)						\
      nhpc_log_debug(fmt, arg1, arg2, arg3, arg4, arg5, arg6)           

#define nhpc_log_debug0(level, fmt, arg1) \
   _nhpc_log_debug(level, fmt, arg1, NULL, NULL, NULL, NULL, NULL)
#define nhpc_log_debug1(level, fmt, arg1, arg2) \
   _nhpc_log_debug(level, fmt, arg1, arg2, NULL, NULL, NULL, NULL)
#define nhpc_log_debug2(level, fmt, arg1, arg2, arg3) \
   _nhpc_log_debug(level, fmt, arg1, arg2, arg3, NULL, NULL, NULL)
#define nhpc_log_debug3(level, fmt, arg1, arg2, arg3, arg4) \
   _nhpc_log_debug(level, fmt, arg1, arg2, arg3, arg4, NULL, NULL)
#define nhpc_log_debug4(level, fmt, arg1, arg2, arg3, arg4, arg5) \
   _nhpc_log_debug(level, fmt, arg1, arg2, arg3, arg4, arg5, NULL)
#define nhpc_log_debug5 _nhpc_log_debug

#define nhpc_log_error nhpc_log_debug

#endif