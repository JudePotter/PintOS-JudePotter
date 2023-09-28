#include "userprog/syscall.h"
#include "userprog/process.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

//#include "lib/user/syscall.c"

#include "devices/shutdown.h"
#include "lib/kernel/list.h"
#include "threads/pte.h"
#include "devices/input.h"
#include "userprog/pagedir.h"
#include "threads/synch.h"
#include "filesys/filesys.h"
#include "filesys/file.h"
#include "threads/palloc.h"

typedef int pid_t;
static struct lock semlock;





// void exit (int status) NO_RETURN;
pid_t exec (const char *file);
// int wait (pid_t);
bool create (const char *file, unsigned initial_size);
bool remove (const char *file);
int open (const char *file);
// int filesize (int fd);
int read (int fd, const void *buffer, unsigned size);

int write_file (int fd, const void *buffer, unsigned length);
// void seek (int fd, unsigned position);
// unsigned tell (int fd);
// void close (int fd);




 // lock_acquire(&semlock);


static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
  lock_init(&semlock);
}


static void
syscall_handler (struct intr_frame *f)
{
  //int code = (int)load_stack(f, ARG);
  //thread_exit();
  int code = *(int*)f->esp;


  // printf("\n\n");
  // printf("System call code:%d\n",*(int*) f->esp);
  // printf ("System call!\n");
  // printf("%s", (const char*)f->esp+1);

  //thread_exit ();

  switch(code)
  {
    case SYS_WRITE:
    {
      int fd = *((int*)f->esp + 1);
      void* buffer = (void*)(*((int*)f->esp + 2));
      unsigned size = *((unsigned*)f->esp + 3);
      //printf("fd: %d, buffer: %p, size: %d",fd, buffer, size);
      f->eax = write_file(fd, buffer, size);
      break;
    }
    case SYS_HALT:                   /* Halt the operating system. */
    {
      printf("Powered off\n");
      shutdown_power_off();
      break;
    }
    case SYS_EXIT:                   /* Terminate this process. */
    {
      int exit_status = *((int*)f->esp + 1);
      struct thread *cur = thread_current();
      char* fname = cur->name; 
      printf("\nexit status:%d, %s", exit_status, fname);
      thread_exit();
      break;
    }


    case SYS_EXEC:                   /* Start another process. */
    {
      f->eax = exec((char *)(*((int*)f->esp+1)));
      break;  
    }

    case SYS_WAIT:                   /* Wait for a child process to die. */
      break;  

    case SYS_CREATE:                 /* Create a file. */
    {
      f->eax = create((char *)(*((int*)f->esp+1)), (unsigned)f->esp+2);
      break;  
    }
    case SYS_REMOVE:                 /* Delete a file. */
    {
      f->eax = remove((char *)(*((int*)f->esp+1)));
      printf("File Removed\n");
      break;  
    }
    case SYS_OPEN:                   /* Open a file. */
      //f->eax = open((char *)(*((int*)f->esp+1)));
      break;  

    case SYS_FILESIZE:               /* Obtain a file's size. */
      break;  

    case SYS_READ:                   /* Read from a file. */
    {
      int file = *((int*)f->esp + 1);
      void* buffer = (void*)(*((int*)f->esp + 2));
      unsigned size = *((unsigned*)f->esp + 3);
      f->eax = read(file, buffer, size);
      break;  
    }

    case SYS_SEEK:                   /* Change position in a file. */
      break;  

    case SYS_TELL:                   /* Report current position in a file. */
      break;  

    case SYS_CLOSE:                  /* Close a file. */
      break;  

    case SYS_MMAP:                   /* Map a file into memory. */
      break;  

    case SYS_MUNMAP:                 /* Remove a memory mapping. */
      break;  

    case SYS_CHDIR:                  /* Change the current directory. */
      break;  

    case SYS_MKDIR:                  /* Create a directory. */
      break;  

    case SYS_READDIR:                /* Reads a directory entry. */
      break;  

    case SYS_ISDIR:                  /* Tests if a fd represents a directory. */
      break;  

    case SYS_INUMBER:               /* Returns the inode number for a fd. */
      break;  

  }
}



bool create(const char *file, unsigned initial_size)
{
  bool success = filesys_create(file, initial_size);
  return success;
}


bool remove(const char *file)
{
  // lock_acquire(&semlock);
  // printf("data is:%s", file);
  bool success = filesys_remove(file);
  // lock_acquire(&semlock);
  return success;
}


pid_t exec(const char *file)
{
  pid_t id = process_execute(file);
  return id;
}


int write_file(int fd, const void* buffer, size_t size)
{
 	if (fd == STDOUT_FILENO)
	{
	  putbuf((const char *)buffer, (size_t)size);
	}
  else printf("Can only output to command line");
  return size;
}


int read(int fd, const void* buffer, unsigned size)
{
  if (fd == 0) // Read from command line
  {
    //printf("Input:");
    for (unsigned i = 0; i < size; i++)
    *((uint8_t *) buffer++) = input_getc();
  }
  else printf("NO"); // Reading from file not setup
  return size;
}



