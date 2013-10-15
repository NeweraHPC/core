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

#include <include/neweraHPC.h>

using namespace std;

nhpc_rbtree_t *parser_options;

void nhpc_http_init_parser() {
   nhpc_rbtree_init(main_pool, &parser_options, RBTREE_STR);
}

nhpc_status_t nhpc_http_parse_status(nhpc_http_request_t *http_request, char *command_str) {
   
}

nhpc_status_t nhpc_http_parse_request_str(nhpc_http_request_t *http_request, char *request_str) {
   int pos;

   http_request->request_str = request_str;
   if((pos = nhpc_strfind(request_str, '?'))) {
   
      char *request_get_variables_str = request_str + pos;
      request_str[pos - 1] = '\0';
      
      nhpc_rbtree_init(http_request->pool, &http_request->request_get_variables, RBTREE_NUM);
      
      char *key   = request_get_variables_str;
      char *value = NULL;
      
      nhpc_key_pair_t *key_pair;
      
      while(1) {
	 switch(*request_get_variables_str) {
	    case '=':
	       if(*(request_get_variables_str + 1) == '\0' || *(request_get_variables_str + 1) == '&')
		  return NHPC_FAIL;
	       
	       *request_get_variables_str = '\0';
	       value = (request_get_variables_str + 1);
	       break;
	       
	    case '&':
	       *request_get_variables_str = '\0';
	       key_pair = (nhpc_key_pair_t *)nhpc_palloc(http_request->pool, sizeof(nhpc_key_pair_t));
	       key_pair->key   = key;
	       key_pair->value = value;
	       nhpc_rbtree_insert(http_request->request_get_variables, key_pair);
	       
	       key   = NULL;
	       value = NULL;
	       
	       break;
	       
	    case '\0':
	       if(key) {
		  key_pair = (nhpc_key_pair_t *)nhpc_palloc(http_request->pool, sizeof(nhpc_key_pair_t));
		  key_pair->key   = key;
		  key_pair->value = value;
		  nhpc_rbtree_insert(http_request->request_get_variables, key_pair);
	       }

	       return NHPC_SUCCESS;
	    default:
	       if(!key)
		  key = request_get_variables_str;
	 }
	 
	 request_get_variables_str++;
      }
   } else {

      http_request->request_get_variables = NULL;
   }

   return NHPC_SUCCESS;
}

nhpc_status_t nhpc_http_parse_http_version(nhpc_http_request_t *http_request, char *http_version) {
   if(*(http_version + 2) == '1')
      http_request->status.version = NHPC_HTTP_VERSION_1_0;
   else 
      http_request->status.version = NHPC_HTTP_VERSION_1_1;
   
   return NHPC_SUCCESS;
}