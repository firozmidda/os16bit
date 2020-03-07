
#include<type.h>
#include<portio.h>

int inpb(int port)
{
    u8 data;
    _asm
    {
        mov dx,port
        in al,dx
        mov data,al
     }
     return (int)data;
}

u16 inpw(int port)
{
    u16 data;
    _asm
    {
        mov dx,port
        in ax,dx
        mov data,ax
     }
     return data;
}     
/*
 int outpb(int port,int data);
  int outpw(int port,u16 data);
*/

int outpb(int port,int data)
{
    u8 value=data;
   _asm{
         mov dx,port
		 mov al,value
         out dx,al		 
       };
       return data;
}
int outpw(int port,u16 data)
{
   _asm{
         mov dx,port
		 mov ax,data
         out dx,ax		 
       };
       return data;
}
/*
boolean inpbuff(int port,void *buffer,int size)
{
     
     int *ptr=(int *)buffer;
     int data=0;   
     for(int i=0;i<size;i+=2)
     {
          _asm 
          {
               mov dx,port
               in ax,dx
               mov data,ax  
          }
          ptr[i]=data;
             
     }  
     return TRUE;
} 
boolean inpbuff(int port,void far *buffer,int size)
{
     
     int far *ptr=(int far *)buffer;
     int data=0;   
     for(int i=0;i<size;i+=2)
     {
          _asm 
          {
               mov dx,port
               in ax,dx
               mov data,ax  
          }
          ptr[i]=data;
             
     }  
     return TRUE;
}                  
*/
