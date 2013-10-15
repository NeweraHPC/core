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

#include <math.h>
#include <stdarg.h>
#include <stdio.h>

#include <include/neweraHPC.h>

nhpc_uint_t log_level = 0;

void nhpc_log_init(const u_char *log_str) {

   if(nhpc_strcmp((const char *)log_str, "debug*") == NHPC_SUCCESS)
      log_str += 5;
   else if(nhpc_strcmp((const char *)log_str, "info") == NHPC_SUCCESS) 
      log_str += 4;
   else 
      log_str = NULL;
   
   if(log_str) {
      
      log_level = nhpc_atoi(log_str);
      if(log_level > LOG_LEVEL_DEBUG_MAX)
	 log_level = LOG_LEVEL_DEBUG_MAX;
      
      log_level = pow(2, log_level + 1) - 1;
   }
}

void nhpc_log_debug(const char *fmt, ...) {
   va_list   ap;
   va_start(ap, fmt);
   vprintf(fmt, ap);
   va_end(ap);
}