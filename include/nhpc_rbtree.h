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

#ifndef _NHPC_RBTREE_H_
#define _NHPC_RBTREE_H_

typedef u_char rb_node_color;
typedef u_char rb_node_mode;

struct rb_node
{
   rb_node        *rb_parent;
   rb_node_color   rb_color;
#define RB_RED               0
#define RB_BLACK             1
   struct rb_node *rb_right;
   struct rb_node *rb_left;
   
   void *data;
   
   union key_t
   {
      int   num;
      char *str;
   };
   key_t key;
};

#define rb_color(r)            ((r)->rb_color)
#define rb_is_red(r)           (!rb_color(r))
#define rb_is_black(r)         rb_color(r)
#define rb_set_red(r)          ((r)->rb_color = RB_RED)
#define rb_set_black(r)        ((r)->rb_color = RB_BLACK)
#define rb_parent(r)           ((r)->rb_parent)
#define rb_set_parent(r, p)    ((r)->rb_parent = p)    
#define rb_set_color(r, color) ((r)->rb_color) = color

#define RBTREE_NUM         1  // 0x0001
#define RBTREE_NUM_MANAGED 2  // 0x0002
#define RBTREE_STR         4  // 0x0004
#define RBTREE_HASH        8  // 0x0008

#define rb_mode_is_num(m)         (m & RBTREE_NUM)
#define rb_mode_is_num_managed(m) (m & RBTREE_NUM_MANAGED) // ((m >> 1) & 1)
#define rb_mode_is_str(m)         (m & RBTREE_STR)         // ((m >> 2) & 1)
#define rb_mode_is_hash(m)        (m & RBTREE_HASH)        // ((m >> 3) & 1)

struct nhpc_rbtree_t
{
   nhpc_pool_t *pool;
   
   rb_node *root;
   
   int          count;
   int          last_assigned_key;
   rb_node_mode rb_mode;
};
void nhpc_rbtree_init(nhpc_pool_t *pool, nhpc_rbtree_t **rbtree, rb_node_mode _rb_mode = RBTREE_NUM);
void nhpc_rbtree_destruct(nhpc_rbtree_t *rbtree);

nhpc_status_t nhpc_rbtree_insert(nhpc_rbtree_t *rbtree, void *data);
nhpc_status_t nhpc_rbtree_insert(nhpc_rbtree_t *rbtree, void *data, int key);
nhpc_status_t nhpc_rbtree_insert(nhpc_rbtree_t *rbtree, void *data, const char *key);

void *nhpc_rbtree_search(nhpc_rbtree_t *rbtree, int key);
void *nhpc_rbtree_search(nhpc_rbtree_t *rbtree, int key, int sub_key);
void *nhpc_rbtree_search(nhpc_rbtree_t *rbtree, const char *key);
void *nhpc_rbtree_search(nhpc_rbtree_t *rbtree, const char *key, int sub_key);
void *nhpc_rbtree_search_inorder_str(nhpc_rbtree_t *rbtree, int pos, const char **key);
void *nhpc_rbtree_search_inorder_num(nhpc_rbtree_t *rbtree, int pos, int *key);

inline int nhpc_rbtree_length(nhpc_rbtree_t *rbtree)
{
   return (*rbtree).count;
}
inline int nhpc_rbtree_length(nhpc_rbtree_t *rbtree, int key)
{
   if(!rb_mode_is_hash((*rbtree).rb_mode))
      return 0;
   
   nhpc_rbtree_t *hash_table = (nhpc_rbtree_t *)nhpc_rbtree_search(rbtree, key);
   if(hash_table)
      return nhpc_rbtree_length(hash_table);
   else 
      return 0;
}

inline int nhpc_rbtree_length(nhpc_rbtree_t *rbtree, const char *key)
{
   if(!rb_mode_is_hash((*rbtree).rb_mode))
      return 0;
   
   nhpc_rbtree_t *hash_table = (nhpc_rbtree_t *)nhpc_rbtree_search(rbtree, key);
   if(hash_table)
      return nhpc_rbtree_length(hash_table);
   else 
      return 0;      
}

nhpc_status_t nhpc_rbtree_erase(nhpc_rbtree_t *rbtree, int key);
nhpc_status_t nhpc_rbtree_erase(nhpc_rbtree_t *rbtree, int key, int subkey);
nhpc_status_t nhpc_rbtree_erase(nhpc_rbtree_t *rbtree, const char *key);
nhpc_status_t nhpc_rbtree_erase(nhpc_rbtree_t *rbtree, const char *key, int subkey);
nhpc_status_t nhpc_rbtree_erase_inorder(nhpc_rbtree_t *rbtree, int pos);

#endif
