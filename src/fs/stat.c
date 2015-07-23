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

PUBLIC int do_stat();

PUBLIC int do_stat()
{
	int fd = fs_msg.FD;
	//void *buf = fs_msg.BUF;
	int pos = pcaller->filp[fd]->fd_pos;

	struct inode buf;
	buf.i_mode =pcaller->filp[fd]->fd_inode->i_mode;
	buf.i_size =  pcaller->filp[fd]->fd_inode->i_size;
	buf.i_start_sect = pcaller->filp[fd]->fd_inode->i_start_sect;
	buf.i_nr_sects = pcaller->filp[fd]->fd_inode->i_nr_sects;
	buf.i_dev = pcaller->filp[fd]->fd_inode->i_dev;
	buf.i_cnt = pcaller->filp[fd]->fd_inode->i_cnt;
	buf.i_num =pcaller->filp[fd]->fd_inode->i_num;
	
	fs_msg.OFFSET = pcaller->filp[fd]->fd_inode->i_size ;
	printf("\nNow stat is working \n");
	printf("i_mode=:%d    ",pcaller->filp[fd]->fd_inode->i_mode);
	printf("i_size=:%d    ",pcaller->filp[fd]->fd_inode->i_size);
	printf("i_start_sect=:%d    ",pcaller->filp[fd]->fd_inode->i_start_sect);
	printf("i_nr_sects=:%d    ",pcaller->filp[fd]->fd_inode->i_nr_sects);
	printf("i_dev=:%d    ",pcaller->filp[fd]->fd_inode->i_dev);
	printf("i_cnt=:%d    ",pcaller->filp[fd]->fd_inode->i_cnt);
	printf("i_num=:\n",pcaller->filp[fd]->fd_inode->i_num);
	return fs_msg.OFFSET;

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

