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

#ifndef _NHPC_STRING_H_
#define _NHPC_STRING_H_

#include "nhpc_general.h"

struct nhpc_str_t {
   u_char      *data;
   nhpc_size_t  len;
};
#define nhpc_str_set(str, text)    (str)->data = (u_char *)text; (str)->len = strlen(text)
#define nhpc_str_init(text)        {(u_char *)text, nhpc_strlen((u_char *)text)}

struct nhpc_key_pair_t {
   char  *key;
   char  *value;
};

#define nhpc_stralloc(s)     ((u_char *)nhpc_alloc(s))
#define nhpc_strpalloc(p, s) ((u_char *)nhpc_palloc(p, s))

void nhpc_strcpy(char *dst, const char *src);
void nhpc_strcpy(char *dst, const char *src, nhpc_size_t len);

nhpc_int_t nhpc_strcmp(const char *s1, const char *s2);

char        *nhpc_itoa(char *dst, int i);
nhpc_uint_t  nhpc_atoi(const u_char *str);

int nhpc_strfind(const char *s1, const char s2);
int nhpc_strfind(const char *s1, const char s2, int old_position);

char *nhpc_substr(char *dst, char *src, int p1, int p2);

#define nhpc_strconcat(...) nhpc_strconcat_va("", ##__VA_ARGS__, NULL)
char *nhpc_strconcat_va(const char *fmt, ...);

#define nhpc_strlen(s)  strlen((const char *)s)

struct nhpc_str_list_data_t {
   nhpc_str_t   *strings;
   nhpc_uint_t   count;
   
   nhpc_str_list_data_t *next;
};

struct nhpc_str_list_t {
   nhpc_pool_t           *pool;
   nhpc_uint_t            max;
   nhpc_str_list_data_t   d;
   nhpc_str_list_data_t  *current;
};

nhpc_str_list_t *nhpc_init_str_list(nhpc_uint_t size, nhpc_pool_t *pool = NULL);
void             nhpc_destroy_str_list(nhpc_str_list_t *list);
void nhpc_add_str_list(nhpc_str_list_t *list, nhpc_str_t str);

#endif