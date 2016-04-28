// Starting code for lab3b

#include <machine_def.h>
#include <string.h>

#define PRINTS_CALL 1
#define EXIT_CALL   2
#define GETS_CALL   6
#define GETI_CALL   7

typedef struct 
{
    int op;
    int addr;
    int status;
} args_t;

int main();
int prints(char * string);
int printi(int val);
int geti();
int gets(char *buff);
int halt();

// interrupt is not needed for this lab, but is needed to compile with -boot
int interrupt() {}

// this is the function that will be called when TRAP instructions execute
// you will have to edit this function to add your functionality
// you will probably also want to change the args
int systrap(args_t * args)
{
    if(args->op == PRINTS_CALL)
    {
        asm("OUTS", args->addr);
    }
    //else if(args->op == EXIT_CALL)
    //{
    //    asm("HALT");
    //}
    else if(args->op == GETS_CALL)
    {
        asm("INP", args);
        while(arg2.op >= 0);
    }
    //else if(args->op == GETI_CALL)
    //{
    //    asm("INP", args);
    //    while(args->op >= 0);
    //}
    //else if(args->op == EXIT_CALL)
    //{
    //  asm("HALT");
    //}
    else 
    { 
        asm("OUTS", "\n");
    }
    asm("RTI");
}

// this is called by user programs to initiate a TRAP
// you will have to edit this function to add your functionality
// you will probably also want to change the args
int syscall(args_t * args)
{
    // Validate the arguments
    
    asm("TRAP");

    // Prepare return
    return 0;
}

int startup__()
{
    // go to user mode
    asm2("SETMODE", FL_USER_MODE); 

    // execute main
    main();

    // terminate the simulator when main is done
    asm("HALT");
}

int sys_prints(char *msg)
{
    asm("OUTS", msg);
    return 0;
}

int halt()
{
    args_t args;                // Argument block
    args.op = EXIT_CALL;
    args.addr = 0;
    syscall(args);
}

int main()
{
    char buff[30];
    int temp;
    // Testing printing functions
    prints("Testing Prints\n");
    //prints("Hello World\n");
    //printi(10);
    //prints("\n");

    // Testing get functions
    //gets(buff);
    //prints(buff);
    //temp = geti();
    //printi(temp);

    //halt();

    return 0;
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
