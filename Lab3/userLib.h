/****************************************************************
* Author: Samuel Huff
* Filename: userLib.h
* Date Created: April 27th, 2016
* Modifications: 
****************************************************************/
#include <string.h>
#define PRINTS_CALL 1
#define EXIT_CALL   2
#define GETS_CALL   6
#define GETI_CALL   7

// Structure for making system calls.
typedef struct 
{
    int op;
    int addr;
    int status;
} args_t;


/********************************************************************** 
* Purpose: Allows user code to access system functions via a trap.
*
* Precondition: 
*   <args> contains information regarding the system call
*   <args>.op is the operation to complete.
*   <args>.addr is the address of the data to/from the systrap.
*   <args>.status is the status of the call.
*
* Postcondition: 
*   User request is serviced by the OS.
*
************************************************************************/
int syscall(args_t * args)
{
    asm("TRAP");

    // Prepare return
    return 0;
}

/********************************************************************** 
* Purpose: Halts the processor
*
* Precondition: 
*   None.
*
* Postcondition: 
*   Execution is halted.
*
************************************************************************/
int halt()
{
    args_t args;                // Argument block
    args.op = EXIT_CALL;
    args.addr = 0;
    syscall(&args);
}

/********************************************************************** 
* Purpose: Prints a string to STDOUT
*
* Precondition: 
*   <string> is the value to display
*
* Postcondition: 
*   <string> is printed to STDOUT
*
************************************************************************/
int prints(char * string)
{
    args_t args;                // Argument block
    args.op = PRINTS_CALL;
    args.addr = string;
    syscall(&args);
    return 0;
}

/********************************************************************** 
* Purpose: Prints an integer to STDOUT
*
* Precondition: 
*   <val> is the integer to print.
*
* Postcondition: 
*   <val> is printed to STDOUT
*
************************************************************************/
int printi(int val)
{
    args_t args;
    char buff[20];      // Buffer to hold converted string value

    itostr(val, buff);
    
    args.op = PRINTS_CALL;
    args.addr = buff;
    syscall(&args);
    return 0;
}

/********************************************************************** 
* Purpose: Gets a string from STDIN.
*
* Precondition: 
*   <buff> is the destination of the string.
*
* Postcondition: 
*   <buff> contains the contents of STDIN. Behaves similar to 
*   fgets(buff, 256, stdin);
*
************************************************************************/
int gets(char *buff)
{
    args_t args;
    args.op = GETS_CALL;
    args.addr = buff;

    syscall(&args);
    return 0;
}

/********************************************************************** 
* Purpose: Gets an integer from STDIN
*
* Precondition: 
*   None.
*
* Postcondition: 
*   Returns an integer to caller.
*
************************************************************************/
int geti()
{
    int temp;
    args_t args;
    args.op = GETI_CALL;
    args.addr = &temp;

    syscall(&args);
    
    return temp;
}
