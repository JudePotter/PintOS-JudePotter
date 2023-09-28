



#include <syscall.h>

int main (void)
{
    char buffer[20];
    read(0, buffer, 20);
    write(1, buffer, 20);

}