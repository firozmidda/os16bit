
#include<type.h>
#include<portio.h>

#ifndef _BIOS_H_
 #define _BIOS_H_




struct WORDREGS
{
     u16 ax,bx,cx,dx,si,di,cflag,flag;
};
struct BYTEREGS
{
     u8 al,ah,bl,bh,cl,ch,dl,dh;
};

union REGS
{
      struct WORDREGS x;
      struct BYTEREGS h;
};

struct  SREGS 
{
    u16 es;
    u16 cs;
    u16 ss;
    u16 ds;
};



int peekb(u32 addr);
int peekw(u32 addr);
void pokeb(u32 addr,u8 data);
void pokew(u32 addr,u16 data);

extern "C" void genint(int n);

int int86(int n,REGS *ir,REGS *or);
int int86x(int n,REGS *ir,REGS *or,SREGS *sr);

 
#endif
 
