

/*
    IBM Port IO
    on  xx-xx-xxxx  -by
                        Firoz Midda
   
*/

#include<type.h>
 
 
#ifndef _PORTIO_H_
  #define _PORTIO_H_
  
  int inpb(int port);
  u16 inpw(int port);
  int outpb(int port,int data);
  int outpw(int port,u16 data);
  
  boolean inpbuff(int port,void *buffer,int size);
  boolean inpbuff(int port,void far *buffer,int size);
  
#endif   
   
