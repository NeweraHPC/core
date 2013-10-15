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

#ifndef _NHPC_FILE_H_
#define _NHPC_FILE_H_

#define NHPC_FILE_NOT_FOUND    1
#define NHPC_FILE              2
#define NHPC_DIRECTORY         4

#define NHPC_VISIBLE_DIR_CHILD 1
#define NHPC_ALL_DIR_CHILD     2

nhpc_status_t nhpc_file_size(const char *file_path, nhpc_size_t *size);
nhpc_status_t nhpc_fileordirectory(const char *file_path);

nhpc_status_t nhpc_get_directorylist(const char *directory_path, nhpc_str_list_t **dir_list, nhpc_pool_t *pool);

#endif
