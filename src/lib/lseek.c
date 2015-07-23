/*************************************************************************//**
 *****************************************************************************
 * @file   lseek.c
 * @brief  
 * @author Forrest Y. Yu
 * @date   Tue Jun  3 16:12:05 2008
 *****************************************************************************
 *****************************************************************************/

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

/*****************************************************************************
 *                                lseek
 *****************************************************************************/
/**
 * Move pointer in file.
 * 
 * @param pathname  The full path of the file to delete.
 * 
 * @return Zero if successful, otherwise -1.
 *****************************************************************************/
PUBLIC int lseek(int fd,int offset,int whence)
{
	MESSAGE msg;
	msg.type   = LSEEK;
	msg.FD = fd;
	msg.OFFSET = offset;
	msg.WHENCE = whence ;

	send_recv(BOTH, TASK_FS, &msg);

	return msg.OFFSET;
}
