#include "types.h"
#include "defs.h"

void
sys_hello(void)
{
    cprintf("Hello\n");
}

int
sys_helloYou(void)
{
    char *name;
    if (argstr(0, &name) < 0)
        return -1;
    cprintf(name);
    return 0;
}