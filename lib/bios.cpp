

#include<type.h>
#include<bios.h>
#include<math.h>
#include<stdio.h>


  
int peekb(u32 addr)
{
    char far *ptr=(char far *)addr;
    return (*ptr);
       
}    

int peekw(u32 addr)
{
    int far *ptr=(int far *)addr;
    return (*ptr) ;
        
}   
void pokeb(u32 addr,u8 data)
{
     char far *ptr=(char far *)addr;
     *ptr=data;
      
}     
void pokew(u32 addr,u16 data) 
{
    int far *ptr=(int far *)addr; 
    *ptr=data;
}     



int int86(int n,REGS *ir,REGS *or)
{
    REGS i,o;
    i=*ir;

    _asm
    {
         push ax
         push bx
         push cx
         push dx
         push si
         push di
         pushf
         
         mov ax,i.x.ax 
         mov bx,i.x.bx 
         mov cx,i.x.cx 
         mov dx,i.x.dx 
         mov si,i.x.si 
         mov di,i.x.di 
     
         push n 
         call genint;
         pop n 
         
         pushf
         pop o.x.flag
                            
         mov o.x.ax,ax
         mov o.x.bx,bx
         mov o.x.cx,cx
         mov o.x.dx,dx
         mov o.x.si,si
         mov o.x.di,di
         
         popf
         pop di
         pop si
         pop dx
         pop cx
         pop bx
         pop ax
    
        
    };
    o.x.cflag=o.x.flag&0x01;
    *or=o;
          
    return 0;
}

int int86x(int n,REGS *ir,REGS *or,SREGS *sr)
{
    REGS i,o;
    i=*ir;
    SREGS s=*sr;
    
    _asm
    {
         push ax
         push bx
         push cx
         push dx
         push si
         push di
         push ds
         push es
         pushf
         
         mov ax,s.es
         mov es,ax
         mov ds,s.ds
         
         mov ax,i.x.ax 
         mov bx,i.x.bx 
         mov cx,i.x.cx 
         mov dx,i.x.dx 
         mov si,i.x.si 
         mov di,i.x.di 
     
         push n 
         call genint;
         pop n 
         
         pushf
         pop o.x.flag
                            
         mov o.x.ax,ax
         mov o.x.bx,bx
         mov o.x.cx,cx
         mov o.x.dx,dx
         mov o.x.si,si
         mov o.x.di,di
         
         popf
         pop ds
         pop es
         pop di
         pop si
         pop dx
         pop cx
         pop bx
         pop ax
    
        
    };
    o.x.cflag=o.x.flag&0x01;
    *or=o;      
    return 0;
}                  
     
                  

