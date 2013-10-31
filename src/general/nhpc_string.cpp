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

#include <stdarg.h>
#include <string.h>
#include <math.h>

#include <include/neweraHPC.h>

void nhpc_strcpy(char *dst, const char *src, nhpc_size_t len) {
   memcpy(dst, src, len);
   dst[len] = '\0';
}

void nhpc_strcpy(char *dst, const char *src) {
   nhpc_strcpy(dst, src, strlen(src));
}

nhpc_int_t nhpc_strcmp(const char *s1, const char *s2) {
   const char *tmp_s1 = s1;
   const char *tmp_s2 = s2;
   bool loop = false;
   const char *tmp_loop_s2;
   
   if(*tmp_s2 == '*')
      while(*(tmp_s2 + 1) == '*')
	 tmp_s2++;
   
   if(*tmp_s2 == '\0')
      return NHPC_SUCCESS;
   
   while(*tmp_s1 != '\0' || *tmp_s2 != '\0') {
      if(*tmp_s1 == *tmp_s2) {
	 tmp_s1++;
	 tmp_s2++;
	 continue;
      }
      else if(*tmp_s1 == '\0' && *tmp_s2 == '*') {
	 while(*tmp_s2 == '*')
	    tmp_s2++;
	 continue;
      }
      else if(*tmp_s2 == '*') {
	 while(*(tmp_s2 + 1) == '*')
	    tmp_s2++;
	 
	 loop = true;
	 tmp_loop_s2 = tmp_s2;
	 
	 while(*tmp_s1 != *(tmp_s2 + 1) && *tmp_s1 != '\0') {
	    *tmp_s1++;
	 }
	 
	 *tmp_s2++;
      }
      else {
	 if(loop != true) {
	    return ((nhpc_int_t)*tmp_s1 - (nhpc_int_t)*tmp_s2);
	 }
	 else {
	    loop = false;
	    tmp_s2 = tmp_loop_s2;
	 }
      }
   }
   
   return NHPC_SUCCESS;
}

char *nhpc_itoa(char *dst, int num) {
   if(num <= 0) {
      dst[0] = '0';
      dst[1] = '\0';
      
      return dst;
   }
   
   int count = 0;
   
   int tmp_num = 1;
   while(num > tmp_num)
   {
      tmp_num = tmp_num * 10;
      count++;
   }
   
   for(int i = 1; i <= count; i++)
   {
      dst[count - i] = (num % 10) + 48;
      num = num / 10;
   }
   dst[count] = '\0';

   return dst;
}

nhpc_uint_t nhpc_atoi(const u_char *str) {

   nhpc_uint_t num = 0;
   nhpc_uint_t tnum;
   nhpc_size_t str_len = nhpc_strlen(str);
   
   while(*str != '\0') {
      
      if((tnum = (nhpc_uint_t)(*str) - 48) > 9) {
	 num = 0;
	 return num;
      }
      
      num += (tnum * pow(10, str_len - 1));
      str_len--;
      str++;
   }

   return num;
}

int nhpc_strfind(const char *s1, const char s2)
{
   return nhpc_strfind(s1, s2, 1);
}

int nhpc_strfind(const char *s1, const char s2, int old_position)
{
   const char *tmp_s1 = s1;
   int position = 0;
   
   old_position--;
   while(*tmp_s1 != '\0' && old_position != 0)
   {
      tmp_s1++;
      old_position--;
      position++;
   }
   
   if(old_position > 0)
   {
      position = 0;
      return position;
   }
   
   while(*tmp_s1 != '\0')
   {
      position++;
      if(*tmp_s1 == s2)
      {
	 return position;
      }	 
      tmp_s1++;
   }
   
   position = 0;
   return position;
}

char *nhpc_substr(char *dst, char *src, int p1, int p2) {
   
}

char *nhpc_strconcat_va(const char *fmt, ...)
{
   nhpc_size_t len = 0;
   
   const char *s;
   char       *string = NULL;      
   va_list     ap;
   
   va_start(ap, fmt);      
   while((s = va_arg(ap, const char *)))
   {
      len += strlen(s);
   }
   va_end(ap);
   
   if(len == 0)
   {
      return NULL;
   }
   string = new char [len + 1];
   string[len] = '\0';
   
   char  *tmp = string;
   
   va_start(ap, fmt);
   while((s = va_arg(ap, const char *)))
   {
      len = strlen(s);
      memcpy((void *)tmp, s, len * sizeof(char));
      tmp += len;
   }      
   va_end(ap);
   
   return string;
}

nhpc_str_list_t *nhpc_init_str_list(nhpc_uint_t size, nhpc_pool_t *pool) {
   
   nhpc_str_list_t *list;
   if(pool) {
      list            = (nhpc_str_list_t *)nhpc_palloc(pool, sizeof(nhpc_str_list_t));
      list->d.strings = (nhpc_str_t *)nhpc_palloc(pool, sizeof(nhpc_str_t) * size); 
   } else {
      list            = (nhpc_str_list_t *)nhpc_alloc(sizeof(nhpc_str_list_t));
      list->d.strings = (nhpc_str_t *)nhpc_alloc(sizeof(nhpc_str_t) * size);
   }

   list->pool    = pool;
   list->max     = size;
   list->d.count = 0;
   list->current = &list->d;
   
   return list;
}

void nhpc_add_str_list(nhpc_str_list_t *list, nhpc_str_t str) {
   
   nhpc_str_list_data_t *list_data;
   if(list->current->count >= list->max) {
      
      if(list->pool) {
	 list_data          = (nhpc_str_list_data_t *)nhpc_palloc(list->pool, sizeof(nhpc_str_list_data_t));
	 list_data->strings = (nhpc_str_t *)nhpc_palloc(list->pool, sizeof(nhpc_str_t) * list->max);
      } else {
	 list_data          = (nhpc_str_list_data_t *)nhpc_alloc(sizeof(nhpc_str_list_data_t));
	 list_data->strings = (nhpc_str_t *)nhpc_alloc(sizeof(nhpc_str_t) * list->max); 
      }
      list_data->count    = 0;
      list->current->next = list_data;
      list->current       = list_data;

   } else {
      list_data = list->current;
   }
   
   memcpy(&list_data->strings[list_data->count], &str, sizeof(nhpc_str_t));
   list_data->count++;
}

nhpc_str_list_t *nhpc_substr(nhpc_pool_t *pool, const char *src, const char tgt, bool ignore_empty, int count) {
   nhpc_str_list_t *list;
   if( count == 0 )
      list = nhpc_init_str_list(10, pool);
   else 
      list = nhpc_init_str_list(count, pool);      
   
   int _count = 0;
   char *new_string ;
   nhpc_size_t  len = 0;
   const char  *_src;
   
   nhpc_str_t str;
   
   do {
      if( !ignore_empty ) {
	 while( *src == tgt )
	    src++;
      }
      _src = src;
      while( *_src != tgt && *_src != '\0' )
	 _src++;
      if( (len = _src - src) > 0 ) {
	 new_string = ( char * )nhpc_palloc(pool, len + 1);
	 memcpy(new_string, src, len);
	 new_string[len] = '\0';
	 _count++;
	 
	 nhpc_str_set(&str, new_string);
	 nhpc_add_str_list(list, str);
      }
      else if( *_src != '\0' ) {
	 nhpc_add_str_list(list, nhpc_str_empty);
	 _src++;
      }
      src = _src;
   }while( ( count == 0 || _count < count ) && *_src != '\0');
   
   return list;
}

nhpc_str_t *nhpc_get_str_list(nhpc_str_list_t *list, nhpc_uint_t n) {
   int count = n / list->max;
   nhpc_str_list_data_t *data = &list->d;
   
   for( int i = 0; i < count && data; i++ )
      data = data->next;
   if( data && (count = n - (count * list->max)) < data->count )
      return &(data->strings[count]);
   else 
      return NULL;
}
