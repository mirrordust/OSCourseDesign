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
#include "proto.h"

PUBLIC int pread(int fd, void *buf, int count,int offset)
{
	MESSAGE msg;
	msg.type = PREAD;
	msg.FD   = fd;
	msg.BUF  = buf;
	msg.CNT  = count;
	msg.OFFSET = offset;

	send_recv(BOTH, TASK_FS, &msg);

	return msg.CNT;
}
