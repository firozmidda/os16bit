
#include<type.h>
#include<math.h>
#include<conv.h>
#include<stdio.h>
/*
u32 shl32(u32 num,int shift)
{
    u16 *ptr,temp;
    ptr=(u16 *)&num;
    
    if(shift<16)
    {
         *ptr=*ptr<<shift;
    }
    else if(shift>=16)
    {
         
         temp=*ptr;
         *ptr=0;
         temp<<(shift%16);
         
         ptr++;
         *ptr=temp;     
    } 
    return num;            
     
                
    
    
}    
*/
