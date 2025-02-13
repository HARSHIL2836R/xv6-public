#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// Custom Calls
int
sys_getNumProc(void)
{
  return getNumProc();
}

int
sys_getMaxPid(void)
{
  return getMaxPid();
}

int
sys_getProcInfo(void)
{
  int pid;
  struct processInfo* pinfo;
  
  if (argint(0,&pid)< 0)
    return -1;
  if (argptr(1,(char **)&pinfo,sizeof pinfo)<0)
    return -1;
  else
    return getProcInfo(pid, pinfo);
}

int
sys_setprio(void)
{
  int pr;
  if (argint(0,&pr)< 0)
    return -1;
  else
    return setprio(pr);
}

int
sys_getprio(void)
{
  return myproc()->pr;
}

int
sys_welcomeFunction(void)
{
  void (*welcome)(void);
  if (argptr(1,(char **)&welcome,sizeof welcome)<0)
    return -1;
  else{
    cprintf("Welcome function set\n");
      myproc()->welcomeFnAdd = welcome;
      myproc()->use_welcome = 1;
    }
  return 0;
}

int
sys_welcomeDone(void)
{
  myproc()->tf->eip = myproc()->welcome_ret_eip;
  // myproc()->context->eip = myproc()->welcome_ret_eip;
  myproc()->use_welcome = 0;
  return 0;
}
