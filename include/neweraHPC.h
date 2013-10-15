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

#ifndef _NEWERAHPC_H_
#define _NEWERAHPC_H_

#include "nhpc_general.h"
#include "nhpc_network.h"

extern nhpc_pool_t *main_pool;

static nhpc_status_t neweraHPC_init(int argc, char **argv) {
   main_pool = nhpc_create_pool(4);
   if(!main_pool)
      return NHPC_FAIL;
   
   nhpc_init_config(argc, argv);
   
   return NHPC_SUCCESS;
}

#endif