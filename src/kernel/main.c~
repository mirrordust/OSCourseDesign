
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            main.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

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


/*======================================================================*
                            kernel_main
 *======================================================================*/
PUBLIC int kernel_main()
{
	disp_str("-----\"kernel_main\" begins-----\n");

	struct task* p_task;
	struct proc* p_proc= proc_table;
	char* p_task_stack = task_stack + STACK_SIZE_TOTAL;
	u16   selector_ldt = SELECTOR_LDT_FIRST;
        u8    privilege;
        u8    rpl;
	int   eflags;
	int   i, j;
	int   prio;
	for (i = 0; i < NR_TASKS+NR_PROCS; i++) {
	        if (i < NR_TASKS) {     /* 任务 */
                        p_task    = task_table + i;
                        privilege = PRIVILEGE_TASK;
                        rpl       = RPL_TASK;
                        eflags    = 0x1202; /* IF=1, IOPL=1, bit 2 is always 1 */
			prio      = 15;
                }
                else {                  /* 用户进程 */
                        p_task    = user_proc_table + (i - NR_TASKS);
                        privilege = PRIVILEGE_USER;
                        rpl       = RPL_USER;
                        eflags    = 0x202; /* IF=1, bit 2 is always 1 */
			prio      = 5;
                }

		strcpy(p_proc->name, p_task->name);	/* name of the process */
		p_proc->pid = i;			/* pid */

		p_proc->ldt_sel = selector_ldt;

		memcpy(&p_proc->ldts[0], &gdt[SELECTOR_KERNEL_CS >> 3],
		       sizeof(struct descriptor));
		p_proc->ldts[0].attr1 = DA_C | privilege << 5;
		memcpy(&p_proc->ldts[1], &gdt[SELECTOR_KERNEL_DS >> 3],
		       sizeof(struct descriptor));
		p_proc->ldts[1].attr1 = DA_DRW | privilege << 5;
		p_proc->regs.cs	= (0 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.ds	= (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.es	= (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.fs	= (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.ss	= (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.gs	= (SELECTOR_KERNEL_GS & SA_RPL_MASK) | rpl;

		p_proc->regs.eip = (u32)p_task->initial_eip;
		p_proc->regs.esp = (u32)p_task_stack;
		p_proc->regs.eflags = eflags;

		p_proc->nr_tty		= 0;

		p_proc->p_flags = 0;
		p_proc->p_msg = 0;
		p_proc->p_recvfrom = NO_TASK;
		p_proc->p_sendto = NO_TASK;
		p_proc->has_int_msg = 0;
		p_proc->q_sending = 0;
		p_proc->next_sending = 0;

		for (j = 0; j < NR_FILES; j++)
			p_proc->filp[j] = 0;

		p_proc->ticks = p_proc->priority = prio;

		p_task_stack -= p_task->stacksize;
		p_proc++;
		p_task++;
		selector_ldt += 1 << 3;
	}

        proc_table[NR_TASKS + 0].nr_tty = 0;
        proc_table[NR_TASKS + 1].nr_tty = 1;
        proc_table[NR_TASKS + 2].nr_tty = 1;

	k_reenter = 0;
	ticks = 0;

	p_proc_ready	= proc_table;

	init_clock();
        init_keyboard();

	restart();

	while(1){}
}


/*****************************************************************************
 *                                get_ticks
 *****************************************************************************/
PUBLIC int get_ticks()
{
	MESSAGE msg;
	reset_msg(&msg);
	msg.type = GET_TICKS;
	send_recv(BOTH, TASK_SYS, &msg);
	return msg.RETVAL;
}


/*======================================================================*
                               TestA
 *======================================================================*/
void TestA()
{
	int fd;
	int i, n;

	char filename[MAX_FILENAME_LEN+1] = "blah";
	const char bufw[] = "abcde";
	const int rd_bytes = 5;
	char bufr[rd_bytes];

	assert(rd_bytes <= strlen(bufw));

	/* create */
	fd = open(filename, O_CREAT | O_RDWR);
	assert(fd != -1);
	printf("File created: %s (fd %d)\n", filename, fd);

	/*char filename2[MAX_FILENAME_LEN+1] = "di";
	fd = open_dir(filename2, O_CREAT | O_RDWR);
	assert(fd != -1);
	printf("File created: %s (fd %d)\n", filename2, fd);

	char filename3[MAX_FILENAME_LEN+1] = "di/f23";
	fd2 = open(filename3, O_CREAT | O_RDWR);
	assert(fd2 != -1);
	printf("File created: %s (fd %d)\n", filename3, fd2);*/
	/*//
	char filename2[MAX_FILENAME_LEN+1] = "bochs";
	fd = open(filename2, O_CREAT | O_RDWR);
	assert(fd != -1);
	printf("File created: %s (fd %d)\n", filename2, fd);
	//*/

	/* write */
	n = write(fd, bufw, strlen(bufw));
	assert(n == strlen(bufw));

	/* close */
	close(fd);

	/* close2 */
	//close(fd2);

	/* open */
	fd = open(filename, O_RDWR);
	assert(fd != -1);
	printf("File opened. fd: %d\n", fd);

	/* open2 */
	//fd = open(filename3, O_RDWR);
	//assert(fd != -1);
	//printf("File opened. fd: %d\n", fd);

	/* read */
	n = read(fd, bufr, rd_bytes);
	assert(n == rd_bytes);
	bufr[n] = 0;
	printf("%d bytes read: %s\n", n, bufr);

/* 新增开始 */

	/*lseek*/
	if(1){
	printf("\nNow lseek is working \n");
	int nice = lseek(fd,0,SEEK_CUR);
	printf("Now lseek is at %d\n", nice);
	printf("Now 5 characters has been written to the file at current position\n");
	n = write(fd, bufw, strlen(bufw));

	nice = lseek(fd,-4,SEEK_END);
	printf("Now lseek is at %d\n", nice);
	n = read(fd, bufr, 5);
	printf("%d bytes read: %s\n", n, bufr);

	nice = lseek(fd,0,SEEK_END);
	printf("Now lseek is at %d\n", nice);
	}

	/* stat */
	if(1){
	struct inode ss;
	n = stat(fd,ss);
	}

	/* dup */
	if(1){
	n = dup(fd);
	}

	/* pwrite and pwread */
	if(1){
	printf("\nNow pwread and pwrite is working\n");
	const char bufws[] = "12345";
	n = pwrite(fd,bufws,5,10);
	printf("At %d,5 bytes pwrite: %s\n", n, bufws);
	n = pread(fd,bufr,5,8);	
	printf("At %d,5 bytes pread: %s\n", n, bufr);
	/*int nice = lseek(fd,10,SEEK_SET);
	printf("Now lseek is at %d\n", nice);*/
	n = read(fd, bufr, 5);
	printf("%d bytes read: %s\n", n, bufr);
	}
	
/* 结束新增 */

	/* close */
	close(fd);

	printf("\n");

	char * filenames[] = {"/foo", "/bar", "/baz"};

	/* create files */
	for (i = 0; i < sizeof(filenames) / sizeof(filenames[0]); i++) {
		fd = open(filenames[i], O_CREAT | O_RDWR);
		assert(fd != -1);
		//printf("File created: %s (fd %d)", filenames[i], fd);
		close(fd);
	}

	char * rfilenames[] = {"/bar", "/foo", "/baz", "/dev_tty0"};

	/* remove files */
	for (i = 0; i < sizeof(rfilenames) / sizeof(rfilenames[0]); i++) {
		if (unlink(rfilenames[i]) == 0){}
			//printf("File removed: %s", rfilenames[i]);
		else{}
			//printf("Failed to remove file: %s", rfilenames[i]);
	}

	spin("TestA");
}

/*======================================================================*
                               TestB
 *======================================================================*/
void TestB()
{
	//printf("%s\n","This is TestB" );
	while(1){
		printf("B");
		milli_delay(200);
	}
}

/*======================================================================*
                               TestB
 *======================================================================*/
void TestC()
{
	//printf("%s\n", "This is TestC");
	/* assert(0); */
	while(1){
		printf("C");
		milli_delay(200);
	}
}

/*****************************************************************************
 *                                panic
 *****************************************************************************/
PUBLIC void panic(const char *fmt, ...)
{
	int i;
	char buf[256];

	/* 4 is the size of fmt in the stack */
	va_list arg = (va_list)((char*)&fmt + 4);

	i = vsprintf(buf, fmt, arg);

	printl("%c !!panic!! %s", MAG_CH_PANIC, buf);

	/* should never arrive here */
	__asm__ __volatile__("ud2");
}

