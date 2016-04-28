// For the last step of the lab, replace your startup__ with the code in this
// file. It will exec() "user.slb".


// The following defines are magic values used by the interpreter.
// DO NOT CHANGE THE VALUES
#define EXEC_CALL   8
#define IO_QUEUED   0x10000000
#define IO_PENDING  0x20000000
#define IO_ERROR    0x40000000
#define IO_COMPLETE 0x80000000
#define BP_REG  0
#define SP_REG  3
#define LP_REG  1
#define FP_REG  4

typedef struct
{
    int op;
    int addr;
    int status;
} io_blk_t;

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
