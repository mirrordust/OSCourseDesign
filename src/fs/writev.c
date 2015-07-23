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

PUBLIC int do_writev();

PUBLIC int do_writev()
{
	return 1;
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

