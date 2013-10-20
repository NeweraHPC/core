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

struct nhpc_config_s {
   nhpc_int_t     workers;
   nhpc_int_t     max_threads;
   nhpc_int_t     max_stack_size;
   nhpc_rbtree_t *options;
   
   nhpc_pool_t   *service_pool;
   nhpc_pool_t   *pool;
};

nhpc_config_s *nhpc_config_init(int argc, char **argv);

static nhpc_rbtree_t *cmdline_arguments;

#define nhpc_get_cmdline_argument(v)           (const char *)(nhpc_rbtree_search(cmdline_arguments, v))
#define nhpc_get_cmdline_argument_key(i, key)  (const char *)(nhpc_rbtree_search_inorder_str(cmdline_arguments, i, &key))
#define nhpc_get_cmdline_argument_count()      (nhpc_get_cmdline_argument_length(cmdline_arguments))
#define nhpc_insert_cmdline_argument(v, key)   (nhpc_rbtree_insert(cmdline_arguments, v, key))
