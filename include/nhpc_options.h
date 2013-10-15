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

static inline int _nhpc_get_int_code(int opt) {
   int i;

   i = 0;
   while(opt > 1) {  
      i++;         
      opt = opt / 2;   
   }               
   
   return i;
}

static inline void nhpc_set_opt(nhpc_status_t *target, int opt, int on) {
   if(!on)
      *target &= ~opt;
   else 
      *target |= opt;
}

static inline bool nhpc_check_opt(nhpc_status_t target, int opt) {
   return ((target >> _nhpc_get_int_code(opt)) & 1);
}
