/****************************************************************
* Author: Philip Howard
* Filename: userLib.h
* Date Created: Unknown
* Modifications: 
*       April 27, 2016 - Samuel Huff
*           Added the systrap function.
****************************************************************/
#include <string.h>

#include <machine_def.h>
#include <string.h>

#define EXEC_CALL   8
#define IO_QUEUED   0x10000000
#define IO_PENDING  0x20000000
#define IO_ERROR    0x40000000
#define IO_COMPLETE 0x80000000
#define PRINTS_CALL 1
#define EXIT_CALL   2
#define GETS_CALL   6
#define GETI_CALL   7

typedef struct
{
    int op;
    int addr;
    int status;
} io_blk_t;

int main();

// interrupt is not needed for this lab, but is needed to compile with -boot
int interrupt() {}

/********************************************************************** 
* Purpose: Performs the systrap to allow users to access OS level stuff.
*
* Precondition: 
*   <args> is the argument for the system call.
*
* Postcondition: 
*   User call is executed in OS level.
*
************************************************************************/
int systrap(io_blk_t * args)
{   
    args->status = asm2("PUSHREG", BP_REG);
    args += args->status;
    args->status = asm2("PUSHREG", BP_REG);
    args->addr += args->status;

    if (args->op == PRINTS_CALL)
    {
        asm("OUTS", args->addr);
    }
    else if (args->op == GETS_CALL)
    {
        asm("INP", args);
        while (args->op >= 0);
    }
    else if (args->op == GETI_CALL)
    {
        asm("INP", args);
        while (args->op >= 0);
    }
    else if (args->op == EXIT_CALL)
    {
        asm("HALT");
    }
    else 
    { 
        asm("OUTS", "Unknown opcode\n");
        asm("NOP");
    }

    asm("RTI");
}

int startup__()
{
    io_blk_t io_blk;

    int bp;
    int high_mem;

    // Set the BP leaving enough room for our stack
    bp = asm2("PUSHREG", SP_REG);
    bp += 64;
    asm2("POPREG", BP_REG, bp);

    // Load user.slb into memory
    io_blk.op = EXEC_CALL;
    io_blk.addr = "user.slb";
    io_blk.status = 0;
    asm("INP", &io_blk);
    while((io_blk.op & IO_COMPLETE) == 0)
    {
    }

    // Set the LP leaving 1000 bytes of stack space
    high_mem = bp + io_blk.status + 1000;
    asm2("POPREG", LP_REG, high_mem);

    // Set SP and FP
    // NOTE: FP must be set LAST!
    high_mem = io_blk.status + 4;
    asm2("POPREG", SP_REG, high_mem);
    asm2("POPREG", FP_REG, high_mem);

    // Execute user.slb
    asm2("JMPUSER", 8); 
    asm("HALT");
}

int sys_prints(char *msg)
{
    asm("OUTS", msg);
    return 0;
}
