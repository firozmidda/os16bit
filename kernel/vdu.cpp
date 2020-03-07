#include<type.h>
#include<bios.h>
#include<portio.h>
#include<vdu.h>



#define BW_SEG     0xb0000000
#define COLOR_SEG  0xb8000000
#define BLANK      0x0700


#define  BW_PORT         0x3B4
#define  COLOR_PORT      0x3D4

#define  INDEX           0x0
#define  DATA            0x01
#define  STATUS          0x06
#define  CURSOR          14


// Global  variables
 
char far  *vid_seg,far *vid_index;

u16 vid_port;
   

int write_monitor(char *str)
{
   char far *p=(char far *)0xB8000000;
   for(int i=0;i<10;i++)
   {
       *p++=*str;
       *p++=0x0f;    
   }     
   return 0;     
    
    
}

int init_vdu()
{
 
      write_monitor("asdasd");
      
      int c_mask;
      c_mask=peekb(0x0410)&0x30;
      if(c_mask==0x30)
      {
          vid_seg=(char far *)BW_SEG;
          vid_port=BW_PORT;
          return 0;
      }
      else 
      {
          vid_seg=(char far *)COLOR_SEG;
           vid_port=COLOR_PORT;
           return 1;
      }
}     

int set_6845(u16 reg,int data)
{
     int port=vid_port; 
     
    outpw(port+INDEX,reg);
    outpb(port+DATA,(data>>8)&0xff);
    
    outpw(port+INDEX,reg+1);
    outpb(port+DATA,data&0xff);
    return 0;
     
}

int write_vdu(u16 row,u16 col,char ch,u16 color)
{          
    vid_index=(vid_seg+row*160+col*2);
    *vid_index=ch;
    vid_index++;
    *vid_index=color;
     
    return 0;  
}
void set_cursor(int row,int colum)
{
     int data=row*80+colum;
     set_6845(CURSOR,data);
}         
                              
void scroll_up()
{
    char far *vidm,far *vidn;
    for(int i=0;i<23;i++)
    {
       for(int j=0;j<160;j++)
       {
           vidm=(vid_seg+i*160+j*2);
           vidn=(vid_seg+(i+1)*160+j*2);
          *vidm=*vidn;
           vidm++; vidn++;
           *vidm=*vidn;
       }
   }
   for(i=0;i<80;i++)
     *(vid_seg+24*160+i*2)=0;
   
   

}
int clear_screen()
{
    int byte=2000;
    long far *vid_ptr=(long far *)vid_seg;
    while(byte)
    {
        *vid_ptr++=0;
        byte--;
    }    
    return 0;  
          
}

