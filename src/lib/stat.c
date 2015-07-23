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

PUBLIC int stat(int fd, void *buf)
{
	MESSAGE msg;
	msg.type = STAT;
	msg.FD = fd;
	msg.BUF  = buf;
	msg.OFFSET = -1;
	

	send_recv(BOTH, TASK_FS, &msg);

	return msg.OFFSET;
}
