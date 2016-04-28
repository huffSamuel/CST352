#include "userLib.h"

int halt()
{
    args_t args;                // Argument block
    args.op = EXIT_CALL;
    args.addr = 0;
    syscall(&args);
}

// Prints a string of text to STDOUT
int prints(char * string)
{
    args_t args;                // Argument block
    args.op = PRINTS_CALL;
    args.addr = string;
    syscall(&args);
    return 0;
}

// Prints an integer to STDOUT as a string
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

// Gets a string from STDIN
int gets(char *buff)
{
    args_t args;
    args.op = GETS_CALL;
    args.addr = buff;

    syscall(&args);
    return 0;
}


// Gets an integer from STDIN
int geti()
{
    int * temp;
    args_t args;
    args.op = GETI_CALL;
    args.addr = temp;

    syscall(&args);
    
    return *temp;
}