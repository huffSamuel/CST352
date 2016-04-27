// Starting code for lab3b

#include <machine_def.h>
#include <string.h>

#define PRINTS_CALL 1
#define EXIT_CALL   2
#define HALT_CALL   3
#define GETL_CALL   6

typedef struct 
{
    int op;
    int addr;
    int status;
} io_blk_t;

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
int systrap(int op, int other_args)
{
    if (op == PRINTS_CALL)
    {
        // perform prints here
        sys_prints(&other_args);
        asm("NOP");
    } else if (op == EXIT_CALL) {
        // halt the CPU
        asm("HALT");
    }
    asm("RTI");
}

// this is called by user programs to initiate a TRAP
// you will have to edit this function to add your functionality
// you will probably also want to change the args
int syscall(io_blk_t io_blk)
{
    printi(io_blk.op);
    printi(io_blk.addr);
    if(io_blk.addr == PRINTS_CALL)
    {
        asm("OUTS", io_blk.addr);
    }
    return 0;
}

int startup__()
{
    // go to user mode
    //asm2("SETMODE", FL_USER_MODE); 

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
    syscall(HALT_CALL, 0);
}

int main()
{
    char buff[256];
    sys_prints("In Main\n");
    //sys_prints("Attempting to trap\n");
    prints("Printed from trap\n");
    //printi(3);
    //printi(10);
    //prints("\n");

    return 0;
}

int prints(char * string)
{
    io_blk_t io_blk;
    io_blk.op = PRINTS_CALL;
    io_blk.addr = string;
    syscall(io_blk);
    return 0;
}

int printi(int val)
{
    io_blk_t io_blk;
    char buff[20];

    itostr(val, buff);
    io_blk.op = PRINTS_CALL;
    io_blk.addr = buff;

    sys_prints(buff);
    return 0;
}