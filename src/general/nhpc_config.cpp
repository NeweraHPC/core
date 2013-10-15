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

#include <stdlib.h>

#include <include/neweraHPC.h>

nhpc_pool_t *main_pool;

void nhpc_init_config(int argc, char **argv) {
   nhpc_rbtree_init((nhpc_pool_t *)main_pool, &cmdline_arguments, RBTREE_STR);
   
   char **tmp_argv = argv + 1;
   while(*tmp_argv != NULL) {
      char *arg = *tmp_argv;
      
      if(*arg != '-') {
	 nhpc_log_error("ERROR: option should begin with '-', invalid option:%s\n", arg);
	 exit(0);
      }
      
      nhpc_insert_cmdline_argument((*(tmp_argv + 1)), (arg + 1));
      
      if(*(tmp_argv + 1)) {
	 if(*(*(tmp_argv + 1)) == '-')
	    tmp_argv += 1;
	 else 
	    tmp_argv += 2;
      } else {
	 tmp_argv += 1;
      }
   }   
   
   const u_char *test_verbose = (const u_char *)nhpc_get_cmdline_argument("v");
   if(test_verbose)
      nhpc_log_init(test_verbose);
}