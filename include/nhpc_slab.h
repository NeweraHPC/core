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

#define NHPC_SLAB_EMPTY    0
#define NHPC_SLAB_OCCUPIED 1

#define nhpc_get_slab_status(p)      ((p)->size & 1)
#define nhpc_set_slab_status(p, s)   ((p)->size 
#define nhpc_set_slab_occupied(p) 

nhpc_slab_t {
   u_char      *start;
   u_char      *end;
   nhpc_size_t  max;
};