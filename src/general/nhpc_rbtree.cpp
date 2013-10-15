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

#include <string.h>

#include <include/nhpc_general.h>

using namespace std;

inline void nhpc_rbtree_rb_link_node(rb_node *new_node, rb_node *parent, rb_node **rb_link)
{
   if(parent)
   {
      new_node->rb_parent = parent;
   }
   
   *rb_link = new_node;
}
void nhpc_rbtree_rb_erase(nhpc_rbtree_t *rbtree, rb_node *node);
void nhpc_rbtree_rb_insert_color(nhpc_rbtree_t *rbtree, rb_node *new_node);
void nhpc_rbtree_rb_erase_color(nhpc_rbtree_t *rbtree, rb_node *node, rb_node *parent);

void __nhpc_rbtree_rb_rotate_left(nhpc_rbtree_t *rbtree, rb_node  *node);
void __nhpc_rbtree_rb_rotate_right(nhpc_rbtree_t *rbtree, rb_node *node);
void __nhpc_rbtree_rb_print(nhpc_rbtree_t *rbtree, rb_node *node);      

nhpc_status_t nhpc_rbtree_insert_node(nhpc_rbtree_t *rbtree, rb_node *node);
rb_node *nhpc_rbtree_search_node(nhpc_rbtree_t *rbtree, int *key_num, const char **key_str);
rb_node *nhpc_rbtree_search_node(nhpc_rbtree_t *rbtree, int pos);
void     nhpc_rbtree_erase_node(nhpc_rbtree_t *rbtree, rb_node *node);

rb_node *nhpc_rbtree_rb_first(nhpc_rbtree_t *rbtree);
rb_node *nhpc_rbtree_rb_last(nhpc_rbtree_t *rbtree);
rb_node *nhpc_rbtree_rb_next(nhpc_rbtree_t *rbtree, const rb_node *node);   

void nhpc_rbtree_init(nhpc_pool_t *pool, nhpc_rbtree_t **rbtree, rb_node_mode _rb_mode)
{
   (*rbtree) = (nhpc_rbtree_t *)nhpc_pcalloc(pool, sizeof(nhpc_rbtree_t));
   
   (*(*rbtree)).pool    = pool;
   (*(*rbtree)).rb_mode = _rb_mode;
}

void nhpc_rbtree_destruct(nhpc_rbtree_t *rbtree)
{
   rb_node *node = nhpc_rbtree_rb_first(rbtree), *tmp = NULL;
   
   while(node || tmp)
   {
      if(tmp)
      {
	 nhpc_rbtree_erase_node(rbtree, tmp);
      }
      tmp  = node;
      if(node)
	 node = nhpc_rbtree_rb_next(rbtree, node);
   }
}

nhpc_status_t nhpc_rbtree_insert_node(nhpc_rbtree_t *rbtree, rb_node *new_node)
{
   rb_node **node = &((*rbtree).root), *parent = NULL;
   int key;
   
   while(*node)
   {
      parent = *node;
      
      if(rb_mode_is_str((*rbtree).rb_mode))
	 key = strcmp(new_node->key.str, ((*node)->key.str));
      else 
	 key = (new_node->key.num) - (*node)->key.num;
      
      if(key < 0)
	 node = &((*node)->rb_left);
      else if(key > 0)
	 node = &((*node)->rb_right);
      else 
      {
	 if(rb_mode_is_hash((*rbtree).rb_mode))
	    break;
	 else if(rb_mode_is_str((*rbtree).rb_mode))
	    nhpc_log_error("ERROR: %s:%s\n", "rbtree_insert() failed, key exists key:", new_node->key.str);
	 else 
	    nhpc_log_error("ERROR: %s:%i\n", "rbtree_insert() failed, key exists key:", new_node->key.num);
	 return NHPC_FAIL;
      }
   }
   
   if(rb_mode_is_hash((*rbtree).rb_mode))
   {
      nhpc_rbtree_t *hash_table;
      void          *data = new_node->data;
      
      if(!(*node))
      {
	 nhpc_rbtree_rb_link_node(new_node, parent, node);
	 nhpc_rbtree_rb_insert_color(rbtree, new_node);
	 
	 ((*rbtree).count)++;
	 
	 nhpc_rbtree_init((*rbtree).pool, &hash_table, RBTREE_NUM_MANAGED);
	 new_node->data = (void *)hash_table;
      }
      else 
      {
	 hash_table = (nhpc_rbtree_t *)(*node)->data;
	 
	 nhpc_pfree((*rbtree).pool, new_node);
      }
      
      nhpc_rbtree_insert(hash_table, data);
   }
   else 
   {
      nhpc_rbtree_rb_link_node(new_node, parent, node);
      nhpc_rbtree_rb_insert_color(rbtree, new_node);
      
      ((*rbtree).count)++;
   }
   
   return NHPC_SUCCESS;
}

rb_node *nhpc_rbtree_search_node(nhpc_rbtree_t *rbtree, int *key_num, const char **key_str)
{
   rb_node *node = (*rbtree).root;
   int key;
   
   while(node)
   {
      if(key_str) 
	 key = nhpc_strcmp(*key_str, node->key.str);
      else 
	 key = *key_num - node->key.num;
      
      if(key < 0)
	 node = node->rb_left;
      else if(key > 0)
	 node = node->rb_right;
      else 
	 break;
   }
   
   if(!node)
   {
      if(key_str)
	 nhpc_log_debug1(LOG_LEVEL_DEBUG_5, "DEBUG: %s:%s\n", "rbtree_search() failed, no key found key", *key_str);
      else 
	 nhpc_log_debug1(LOG_LEVEL_DEBUG_5, "DEBUG: %s:%i\n", "rbtree_search() failed, no key found key", *key_num);
   }
   
   return node;
}

rb_node *nhpc_rbtree_search_node(nhpc_rbtree_t *rbtree, int pos)
{
   rb_node *node = NULL;
   for(int i = 1; i <= pos; i++) {
      if(!node)
	 node = nhpc_rbtree_rb_first(rbtree);
      else 
	 node = nhpc_rbtree_rb_next(rbtree, node);
   }
   
   if(!node)
      nhpc_log_debug1(LOG_LEVEL_DEBUG_5, "DEBUG: %s:%i\n", "rbtree_search_inorder() failed, no key found key", pos);
   
   return node;
}

void nhpc_rbtree_erase_node(nhpc_rbtree_t *rbtree, rb_node *node)
{
   if(rb_mode_is_num_managed((*rbtree).rb_mode))
   {
      rb_node *tmp = node;
      while((tmp = nhpc_rbtree_rb_next(rbtree, tmp)))
	 tmp->key.num -= 1;
      
      (*rbtree).last_assigned_key--;
   }
   
   nhpc_rbtree_rb_erase(rbtree, node);
   
   if(rb_mode_is_str((*rbtree).rb_mode))
      delete[] node->key.str;
   if(rb_mode_is_hash((*rbtree).rb_mode))
      nhpc_rbtree_destruct((nhpc_rbtree_t *)(node->data));
   
   ((*rbtree).count)--;
   
   nhpc_pfree((*rbtree).pool, node);
}

nhpc_status_t nhpc_rbtree_insert(nhpc_rbtree_t *rbtree, void *data)
{
   if(rb_mode_is_str((*rbtree).rb_mode))
      return NHPC_FAIL;
   
   return nhpc_rbtree_insert(rbtree, data, ((*rbtree).last_assigned_key + 1));
}

nhpc_status_t nhpc_rbtree_insert(nhpc_rbtree_t *rbtree, void *data, int key)
{
   if(rb_mode_is_str((*rbtree).rb_mode))
      return NHPC_FAIL;
   
   rb_node *new_node = (rb_node *)nhpc_pcalloc((*rbtree).pool, sizeof(rb_node));
   new_node->data    = data;
   new_node->key.num = key;
   
   nhpc_status_t nrv;
   nrv = nhpc_rbtree_insert_node(rbtree, new_node);
   if(key > (*rbtree).last_assigned_key)
      (*rbtree).last_assigned_key = key;
   
   if(nrv == NHPC_FAIL)
   {
      nhpc_pfree((*rbtree).pool, new_node);
      return NHPC_FAIL;
   }
   
   return (new_node->key.num);
}

nhpc_status_t nhpc_rbtree_insert(nhpc_rbtree_t *rbtree, void *data, const char *key)
{
   if(!rb_mode_is_str((*rbtree).rb_mode))
      return NHPC_FAIL;
   
   rb_node *new_node = (rb_node *)nhpc_pcalloc((*rbtree).pool, sizeof(rb_node));
   new_node->data    = data;
   new_node->key.str = (char *)key;
   
   nhpc_status_t nrv;
   nrv = nhpc_rbtree_insert_node(rbtree, new_node);
   
   if(nrv == NHPC_FAIL)
   {
      nhpc_pfree((*rbtree).pool, (new_node));
   }
   
   return nrv;
}

void *nhpc_rbtree_search(nhpc_rbtree_t *rbtree, int key)
{
   if(rb_mode_is_str((*rbtree).rb_mode))
      return NULL;
   
   rb_node *node = nhpc_rbtree_search_node(rbtree, &key, NULL);
   if(node)
      return node->data;
   else 
      return NULL;
}

void *nhpc_rbtree_search(nhpc_rbtree_t *rbtree, int key, int subkey)
{
   if(!rb_mode_is_hash((*rbtree).rb_mode))
      return NULL;
   
   nhpc_rbtree_t *hash_table = (nhpc_rbtree_t *)nhpc_rbtree_search(rbtree, key);
   if(!hash_table)
      return NULL;
   
   void *data = nhpc_rbtree_search(rbtree, subkey);
   
   return data;
}

void *nhpc_rbtree_search(nhpc_rbtree_t *rbtree, const char *key)
{
   if(!rb_mode_is_str((*rbtree).rb_mode))
      return NULL;
   
   rb_node *node = nhpc_rbtree_search_node(rbtree, NULL, &key);
   if(node)
      return node->data;
   else 
      return NULL;      
}

void *nhpc_rbtree_search(nhpc_rbtree_t *rbtree, const char *key, int subkey)
{
   if(!rb_mode_is_hash((*rbtree).rb_mode))
      return NULL;
   
   nhpc_rbtree_t *hash_table = (nhpc_rbtree_t *)nhpc_rbtree_search(rbtree, key);
   if(!hash_table)
      return NULL;
   
   void *data = nhpc_rbtree_search(rbtree, subkey);
   
   return data;
}

void *nhpc_rbtree_search_inorder_str(nhpc_rbtree_t *rbtree, int pos, const char **key)
{
   if(!rb_mode_is_str((*rbtree).rb_mode))
      return NULL;
   
   rb_node *node = nhpc_rbtree_search_node(rbtree, pos);
   
   if(node)
   {
      if(key)
	 *key = (node->key.str);
      return node->data;
   }
   else 
      return NULL;
}

void *nhpc_rbtree_search_inorder_num(nhpc_rbtree_t *rbtree, int pos, int *key)
{
   if(rb_mode_is_str((*rbtree).rb_mode))
      return NULL;
   
   rb_node *node = nhpc_rbtree_search_node(rbtree, pos);
   
   if(node)
   {
      if(key)
	 *key = node->key.num;
      return node->data;
   }
   else 
      return NULL;
}

nhpc_status_t nhpc_rbtree_erase(nhpc_rbtree_t *rbtree, int key)
{
   if(rb_mode_is_str((*rbtree).rb_mode))
      return NHPC_FAIL;
   
   rb_node *node = nhpc_rbtree_search_node(rbtree, &key, NULL);
   if(node)
   {
      nhpc_rbtree_erase_node(rbtree, node);
      return NHPC_SUCCESS;
   }
   
   return NHPC_FAIL;
}

nhpc_status_t nhpc_rbtree_erase(nhpc_rbtree_t *rbtree, int key, int subkey)
{
   if(!rb_mode_is_hash((*rbtree).rb_mode))
      return NHPC_FAIL;
   
   nhpc_rbtree_t *hash_table = (nhpc_rbtree_t *)nhpc_rbtree_search(rbtree, key);
   if(!hash_table)
      return NHPC_FAIL;
   
   return (nhpc_rbtree_erase(hash_table, subkey));
}

nhpc_status_t nhpc_rbtree_erase(nhpc_rbtree_t *rbtree, const char *key)
{
   if(!rb_mode_is_str((*rbtree).rb_mode))
      return NHPC_FAIL;
   
   rb_node *node = nhpc_rbtree_search_node(rbtree, NULL, &key);
   if(node)
   {
      nhpc_rbtree_erase_node(rbtree, node);
      return NHPC_SUCCESS;
   }
   
   return NHPC_FAIL;
}

nhpc_status_t nhpc_rbtree_erase(nhpc_rbtree_t *rbtree, const char *key, int subkey)
{
   if(!rb_mode_is_hash((*rbtree).rb_mode))
      return NHPC_FAIL;
   
   nhpc_rbtree_t *hash_table = (nhpc_rbtree_t *)nhpc_rbtree_search(rbtree, key);
   if(!hash_table)
      return NHPC_FAIL;
   
   return (nhpc_rbtree_erase(hash_table, subkey));
}

nhpc_status_t nhpc_rbtree_erase_inorder(nhpc_rbtree_t *rbtree, int pos)
{
   rb_node *node = nhpc_rbtree_search_node(rbtree, pos);
   
   if(node)
   {
      nhpc_rbtree_erase_node(rbtree, node);
      return NHPC_SUCCESS;
   }
   
   return NHPC_FAIL;
}

void __nhpc_rbtree_rb_rotate_left(nhpc_rbtree_t *rbtree, struct rb_node *node)
{
   struct rb_node *right = node->rb_right;
   struct rb_node *parent = rb_parent(node);
   
   if((node->rb_right = right->rb_left))
      rb_set_parent(right->rb_left, node);
   right->rb_left = node;
   
   rb_set_parent(right, parent);
   
   if(parent)
   {
      if(node == parent->rb_left)
	 parent->rb_left = right;
      else
	 parent->rb_right = right;
   }
   else
      (*rbtree).root = right;
   rb_set_parent(node, right);
}

void __nhpc_rbtree_rb_rotate_right(nhpc_rbtree_t *rbtree, struct rb_node *node)
{
   struct rb_node *left = node->rb_left;
   struct rb_node *parent = rb_parent(node);
   
   if ((node->rb_left = left->rb_right))
      rb_set_parent(left->rb_right, node);
   left->rb_right = node;
   
   rb_set_parent(left, parent);
   
   if (parent)
   {
      if (node == parent->rb_right)
	 parent->rb_right = left;
      else
	 parent->rb_left = left;
   }
   else
      (*rbtree).root = left;
   rb_set_parent(node, left);
}

void nhpc_rbtree_rb_insert_color(nhpc_rbtree_t *rbtree, struct rb_node *node)
{
   struct rb_node *parent, *gparent;
   
   while((parent = rb_parent(node)) && rb_is_red(parent))
   {
      gparent = rb_parent(parent);
      
      if(parent == gparent->rb_left)
      {
	 {
	    register struct rb_node *uncle = gparent->rb_right;
	    if(uncle && rb_is_red(uncle))
	    {
	       rb_set_black(uncle);
	       rb_set_black(parent);
	       rb_set_red(gparent);
	       node = gparent;
	       continue;
	    }
	 }
	 
	 if(parent->rb_right == node)
	 {
	    register struct rb_node *tmp;
	    __nhpc_rbtree_rb_rotate_left(rbtree, parent);
	    tmp = parent;
	    parent = node;
	    node = tmp;
	 }
	 
	 rb_set_black(parent);
	 rb_set_red(gparent);
	 __nhpc_rbtree_rb_rotate_right(rbtree, gparent);
      } 
      else 
      {
	 {
	    register struct rb_node *uncle = gparent->rb_left;
	    if(uncle && rb_is_red(uncle))
	    {
	       rb_set_black(uncle);
	       rb_set_black(parent);
	       rb_set_red(gparent);
	       node = gparent;
	       continue;
	    }
	 }
	 
	 if(parent->rb_left == node)
	 {
	    register struct rb_node *tmp;
	    __nhpc_rbtree_rb_rotate_right(rbtree, parent);
	    tmp = parent;
	    parent = node;
	    node = tmp;
	 }
	 
	 rb_set_black(parent);
	 rb_set_red(gparent);
	 __nhpc_rbtree_rb_rotate_left(rbtree, gparent);
      }
   }
   
   rb_set_black((*rbtree).root);
}

void nhpc_rbtree_rb_erase_color(nhpc_rbtree_t *rbtree, struct rb_node *node, struct rb_node *parent)
{
   struct rb_node *other;
   
   while((!node || rb_is_black(node)) && node != (*rbtree).root)
   {
      if(parent->rb_left == node)
      {
	 other = parent->rb_right;
	 if(rb_is_red(other))
	 {
	    rb_set_black(other);
	    rb_set_red(parent);
	    __nhpc_rbtree_rb_rotate_left(rbtree, parent);
	    other = parent->rb_right;
	 }
	 if((!other->rb_left || rb_is_black(other->rb_left)) &&
	    (!other->rb_right || rb_is_black(other->rb_right)))
	 {
	    rb_set_red(other);
	    node = parent;
	    parent = rb_parent(node);
	 }
	 else
	 {
	    if(!other->rb_right || rb_is_black(other->rb_right))
	    {
	       rb_set_black(other->rb_left);
	       rb_set_red(other);
	       __nhpc_rbtree_rb_rotate_right(rbtree, other);
	       other = parent->rb_right;
	    }
	    rb_set_color(other, rb_color(parent));
	    rb_set_black(parent);
	    rb_set_black(other->rb_right);
	    __nhpc_rbtree_rb_rotate_left(rbtree, parent);
	    node = (*rbtree).root;
	    break;
	 }
      }
      else
      {
	 other = parent->rb_left;
	 if(rb_is_red(other))
	 {
	    rb_set_black(other);
	    rb_set_red(parent);
	    __nhpc_rbtree_rb_rotate_right(rbtree, parent);
	    other = parent->rb_left;
	 }
	 if((!other->rb_left || rb_is_black(other->rb_left)) &&
	    (!other->rb_right || rb_is_black(other->rb_right)))
	 {
	    rb_set_red(other);
	    node = parent;
	    parent = rb_parent(node);
	 }
	 else
	 {
	    if(!other->rb_left || rb_is_black(other->rb_left))
	    {
	       rb_set_black(other->rb_right);
	       rb_set_red(other);
	       __nhpc_rbtree_rb_rotate_left(rbtree, other);
	       other = parent->rb_left;
	    }
	    rb_set_color(other, rb_color(parent));
	    rb_set_black(parent);
	    rb_set_black(other->rb_left);
	    __nhpc_rbtree_rb_rotate_right(rbtree, parent);
	    node = (*rbtree).root;
	    break;
	 }
      }
   }
   if(node)
      rb_set_black(node);
}

void nhpc_rbtree_rb_erase(nhpc_rbtree_t *rbtree, struct rb_node *node)
{
   struct rb_node *child, *parent;
   int color;
   
   if(!node->rb_left)
      child = node->rb_right;
   else if(!node->rb_right)
      child = node->rb_left;
   else
   {
      struct rb_node *old = node, *left;
      
      node = node->rb_right;
      while((left = node->rb_left) != NULL)
	 node = left;
      
      if(rb_parent(old))
      {
	 if (rb_parent(old)->rb_left == old)
	    rb_parent(old)->rb_left = node;
	 else
	    rb_parent(old)->rb_right = node;
      } else
	 (*rbtree).root = node;
      
      child = node->rb_right;
      parent = rb_parent(node);
      color = rb_color(node);
      
      if (parent == old)
      {
	 parent = node;
      } 
      else 
      {
	 if (child)
	    rb_set_parent(child, parent);
	 parent->rb_left = child;
	 
	 node->rb_right = old->rb_right;
	 rb_set_parent(old->rb_right, node);
      }
      
      rb_set_parent(node, rb_parent(old));
      rb_set_color(node, rb_color(old));
      node->rb_left = old->rb_left;
      rb_set_parent(old->rb_left, node);
      
      goto color;
   }
   
   parent = rb_parent(node);
   color = rb_color(node);
   
   if(child)
      rb_set_parent(child, parent);
   if(parent)
   {
      if (parent->rb_left == node)
	 parent->rb_left = child;
      else
	 parent->rb_right = child;
   }
   else
      (*rbtree).root = child;
   
color:
   if(color == RB_BLACK)
      nhpc_rbtree_rb_erase_color(rbtree, child, parent);
}

rb_node *nhpc_rbtree_rb_first(nhpc_rbtree_t *rbtree)
{
   rb_node *n = (*rbtree).root;
   
   if (!n)
      return NULL;
   while (n->rb_left)
      n = n->rb_left;
   return n;
}

rb_node *nhpc_rbtree_rb_last(nhpc_rbtree_t *rbtree)
{
   rb_node *n = (*rbtree).root;
   
   if (!n)
      return NULL;
   while (n->rb_right)
      n = n->rb_right;
   return n;
}

rb_node *nhpc_rbtree_rb_next(nhpc_rbtree_t *rbtree, const rb_node *node)
{
   rb_node *parent;
   
   if(node->rb_parent == node)
      return NULL;
   
   if(node->rb_right) 
   {
      node = node->rb_right; 
      while(node->rb_left)
	 node=node->rb_left;
      return (struct rb_node *)node;
   }
   
   while((parent = node->rb_parent) && node == parent->rb_right)
      node = parent;
   
   return parent;
}
