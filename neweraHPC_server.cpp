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

#include <stdio.h>

#include <include/neweraHPC.h>

int main(int argc, char **argv)
{
   nhpc_status_t nrv;
   
   neweraHPC_init(argc, argv);
   
   nhpc_config_t *main_config = nhpc_config_init(argc, argv);
   if( !main_config )
      return 1;
   if( nhpc_init_threads(main_config) != NHPC_SUCCESS )
      return 1;
   if( nhpc_init_workers(main_config) != NHPC_SUCCESS )
      return 1;
   if( nhpc_init_connections(main_config) != NHPC_SUCCESS )
      return 1;
   
   nhpc_listening_t *listener;
   if( nhpc_create_listening_server(&listener, main_config, "localhost", "8080") != NHPC_SUCCESS )
      return 1;
}