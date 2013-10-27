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
#include <errno.h>
#include <string.h>
#include <stdio.h>

#include <include/neweraHPC.h>

nhpc_pool_t *main_pool;

int nhpc_config_read_file(nhpc_config_t **config, nhpc_str_t *config_file_path) {
   FILE *fp;
   fp = fopen(( const char * )config_file_path->data, "r");
   if( !fp ) {
      nhpc_log_error("ERROR: Unable to open config file '%s': %s\n", config_file_path->data, strerror(errno));        
   }
   
   nhpc_int_t  line_no  = 0;
   nhpc_size_t line_len = 12;

   char  *line_str = NULL;
   char   buffer[100];
   
   while( (line_str = fgets(buffer, 100, fp)) ) {
      line_no++;      
      
      while( *line_str == ' ' )
	 line_str++;
      if( *line_str == '#' )
	 continue;

      if( strlen(line_str) == 1 )
	 continue;
      else if( strlen(line_str) == 99 )
	 return NHPC_FAIL;
      
      {
	 char *opt_str = line_str;
	 
	 int n = 0;
	 while( *opt_str != ' ' && *opt_str != '=' && *opt_str != '\0' && *opt_str != '\n' && *opt_str != '\r' ) {
	    opt_str++;
	    n++;
	 }
	 
	 char *option = ( char * )nhpc_palloc((*config)->service_pool, n + 1);
	 char *value  = NULL;
	 
	 nhpc_strcpy(option, (opt_str - n), n);
	 option[n] = '\0';
	 
	 while( *opt_str == ' ' )
	    opt_str++;
	 if( *opt_str == '\0' || *opt_str == '\n' || *opt_str == '\r' )
	    nhpc_rbtree_insert((*config)->options, &nhpc_str_empty, option);
	 else if( *opt_str == '=' ) {
	    opt_str++;
	    while( *opt_str == ' ' ) {
	       opt_str++;
	    }
	    n = 0;
	    while( *opt_str != ' ' && *opt_str != '\0' && *opt_str != '\n' && *opt_str != '\r' ) {
	       opt_str++;
	       n++;
	    }
	    if( n == 0 ) {
	       nhpc_log_error("ERROR: Config invalid option at line %i\n", line_no);
	       exit(1);
	    }
	    
	    value = ( char * )nhpc_palloc((*config)->service_pool, n + 1);
	    nhpc_strcpy(value, (opt_str - n), n);
	    value[n] = '\0';

	    nhpc_str_t *arg_val = ( nhpc_str_t * )nhpc_palloc((*config)->service_pool, sizeof( nhpc_str_t ));
	    nhpc_str_set(arg_val, value);
	    nhpc_rbtree_insert((*config)->options, arg_val, option);	 	    
	 }
      }
   }
   
   return NHPC_SUCCESS;
}

nhpc_config_t *nhpc_config_init(int argc, char **argv) {
   nhpc_config_t *config = ( nhpc_config_t * )nhpc_calloc(sizeof(nhpc_config_t));
   config->service_pool = nhpc_create_pool(1);
   
   nhpc_rbtree_init(config->service_pool, &config->options, RBTREE_STR);
   
   char **tmp_argv = argv + 1;
   char  *arg;
   
   while( *tmp_argv != NULL ) {
      arg = *tmp_argv;
      if( *arg != '-' ) {
	 nhpc_log_error("ERROR: option should begin with '-', invalid option:%s\n", arg);
	 exit(0);	 
      }
      if( *(arg + 1) == '-') 
	 arg += 2;
      else {
	 arg += 1;
	 if( *arg == '\0' )
	    return NULL;
      }
      
      (tmp_argv) += 1;

      if( *tmp_argv != NULL && *(*tmp_argv) != '-' ) {
	 nhpc_str_t *arg_val = ( nhpc_str_t * )nhpc_palloc(config->service_pool, sizeof( nhpc_str_t ));
	 nhpc_str_set(arg_val, (*tmp_argv));
	 nhpc_rbtree_insert(config->options, arg_val, arg);	 
	 tmp_argv += 1;
      } else 
	 nhpc_rbtree_insert(config->options, &nhpc_str_empty, arg);
   }
   
   /* All the command line options have been parsed, now looking for config file */
   nhpc_str_t *test_config_file;
   if( ((test_config_file = ( nhpc_str_t * )nhpc_rbtree_search(config->options, "c")) || 
	(test_config_file = ( nhpc_str_t * )nhpc_rbtree_search(config->options, "config"))) && 
       test_config_file->len > 0 ) {
      if( nhpc_config_read_file(&config, test_config_file) != NHPC_SUCCESS )
	 return NULL;
   }
 
   nhpc_str_t *test_verbose;
   if( ((test_verbose = ( nhpc_str_t * )nhpc_rbtree_search(config->options, "v")) || 
	(test_verbose = ( nhpc_str_t * )nhpc_rbtree_search(config->options, "verbose"))) &&
       test_verbose->len > 0 ) {
      nhpc_log_init(test_verbose->data);
   }
   
   return config;
}