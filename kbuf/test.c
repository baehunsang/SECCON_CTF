#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> 
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>

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

int main(){
    int fd = open("/dev/kbuf", O_RDWR);
    lseek(fd, -0x100, SEEK_END);
    read(fd, buf, 0x100);
    hexdump(buf, 0x100);
}