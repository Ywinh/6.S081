#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "sysinfo.h"  //add in lab2 sysinfo

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
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

uint64
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

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
//add
uint64
sys_trace(void){
  int n;

  if(argint(0,&n) < 0)
    return -1;
  myproc()->sys_num = n;//读取参数中的跟踪掩码
  return 0;
}

//add
uint64
sys_sysinfo(void){
  //将一个struct sysinfo从内核复制回用户空间
  struct sysinfo info;
  struct proc* p = myproc();
  uint64 st;//要让st指向用户空间的地址

  /*if(argint(0,&info) < 0)
    return -1;*/
  if(argaddr(0,&st) < 0)
    return -1;
  
  //抄的，之前妄想从通过argint读取info参数
  info.freemem = freemem();
  info.nproc = num_proc();

  if(copyout(p->pagetable,st,(char*)&info, sizeof(info)) < 0)
    return -1;
  

  return 0;

}