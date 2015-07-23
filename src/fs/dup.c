#include "type.h"
#include "stdio.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "fs.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "keyboard.h"
#include "proto.h"

PUBLIC int do_dup();

PUBLIC int do_dup()
{
	printf("\n");
	int i;
	int fd = fs_msg.FD;
	int flags = fs_msg.FLAGS;	/* access mode */
	int name_len = fs_msg.NAME_LEN;	/* length of filename */
	int src = fs_msg.source;	/* caller proc nr. */

	char pathname[MAX_PATH];

	int inode_nr = search_file(pathname);

	if (!(pcaller->filp[fd]->fd_mode & O_RDWR))
		return 0;

	struct inode * pin;
	struct inode * dir_inode;
	pin = get_inode(dir_inode->i_dev, inode_nr);

	/* find a free slot in f_desc_table[] */
	for (i = 0; i < NR_FILE_DESC; i++)
		if (f_desc_table[i].fd_inode == 0)
			break;


	int fd_pos = pcaller->filp[fd]-> fd_pos;
	int fd_mode = pcaller->filp[fd]-> fd_mode;
	int fd_inode = pcaller->filp[fd]-> fd_inode;
	/*struct file_desc *fileDescription; 
	fileDescription->fd_mode = fd_mode;
	fileDescription->fd_pos = pos;
	fileDescription->fd_inode = fd_inode; */
	printf("Now dup is working\n");
	printf("Before duplication,%d is the first one that is not a file description\n",i);
	pcaller->filp[i] = &f_desc_table[i];
	f_desc_table[i].fd_inode = fd_inode;
	f_desc_table[i].fd_mode = fd_mode;
	f_desc_table[i].fd_pos = fd_pos;

	for (i = 0; i < NR_FILE_DESC; i++)
		if (f_desc_table[i].fd_inode == 0)
			break;
	printf("After duplication,%d is the first one that is not a file description\n",i);
	
	
	return fd;
	/*int fd = fs_msg.FD;
	int off = fs_msg.OFFSET;
	int whence = fs_msg.WHENCE;

	int pos = pcaller->filp[fd]->fd_pos;
	int f_size = pcaller->filp[fd]->fd_inode->i_size;

	switch (whence) {
	case SEEK_SET:
		pos = off;
		break;
	case SEEK_CUR:
		pos += off;
		break;
	case SEEK_END:
		pos = f_size + off;
		break;
	default:
		return -1;
		break;
	}
	if ((pos > f_size) || (pos < 0)) {
		return -1;
	}
	pcaller->filp[fd]->fd_pos = pos;
	return pos;*/
}

