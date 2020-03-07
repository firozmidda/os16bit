
#include<type.h>
#include<vdu.h>
#include<bios.h>
#include<conv.h>


#define BW_SEG     0xb0000000
#define COLOR_SEG  0xb8000000
#define BLANK      0x0700


#define  BW_PORT         0x3B4
#define  COLOR_PORT      0x3D4

#define  INDEX           0x0
#define  DATA            0x01
#define  STATUS          0x06
#define  CURSOR          14

// global variable 

char far *vid_seg,far *vid_index;
u16 console_buff[100];
int console_buff_pos=0;
int frow=0,fcolum=0;


int vid_port;
int row=0,colum=0;



int write_vdu(int row,int col,char *str)
{
     vid_index=(vid_seg+row*160+col*2);
     int i=0;
     while(str[i])
     {
        *vid_index=str[i++];
         vid_index++;
        *vid_index=0x0f;
         vid_index++;
     }  
     return i; 
   
}    

int flush_buff()
{
    
    int i=0;
    u16 far *vid_ptr=(u16 far *)vid_seg;
   
    vid_ptr+=(frow*80+fcolum);
    
    while(console_buff_pos)
    {
        *vid_ptr++=console_buff[i++];
        console_buff_pos--;
    }
    frow=row;
    fcolum=colum;
    
    return 0;
                          
}    
   

int init_vdu()
{
 
      int c_mask;
      int rc;
      //colum=peekb(0x450);
      //row=peekb(0x451);
      c_mask=peekb(0x410)&0x30;
      if(c_mask==0x30)
      {
          vid_seg=(char far *)BW_SEG;
          vid_port=BW_PORT;
          write_vdu(row++,colum,"BLACK AND WHITE MONITER ");
         
      }
      else 
      {
          vid_seg=(char far *)COLOR_SEG;
          vid_port=COLOR_PORT;
          write_vdu(row++,colum,"COLOR MONITER ");
         
      }
      return rc;
}     

int scroll_up(int line)
{
   
   int far *vid_ptr=(int far *)vid_seg;
   int far *vid_ptr2=vid_ptr;
   vid_ptr2+=(line*80);
   int byte=(row-line)+1;
   
   byte *=80;
   while(byte)
   {
       *vid_ptr++=*vid_ptr2++;
       byte--;
       
   }
   row-=line;
   return 0;
    
}         

int put_buff(char ch,int color)
{
    char far *vid_ptr=(char far *)vid_seg;
    switch(ch)
    {
        
        case '\b':
             if(colum!=0) colum--;
                vid_ptr+=(row*160+colum*2);
                *vid_ptr++=' ';
             break;
        case '\t':
             colum+=4;     
             break;
        case '\r':
             colum=0;   
             break;
        case '\n':
             colum=0;  row++; 
             break;
        default:
             if(colum>=80)
             {
                 row++;
                 colum=0;
             }
             else
              colum++;
             if(row>24)
              scroll_up(2);
             vid_ptr+=(row*160+colum*2);
             *vid_ptr++=ch;
             *vid_ptr=color; 
             
    } 
    return 0;         
    
} 



/*
int put_buff(char ch,int color)
{
    
    switch(ch)
    {
        
        case '\b':
             if(colum!=0)   console_buff[console_buff_pos++]=(' '|color<<8); flush_buff(); colum--;
             break;
        case '\t':
             colum+=4;    flush_buff(); 
             break;
        case '\r':
             colum=0;   flush_buff();
             break;
        case '\n':
             colum=0;  row++; flush_buff();
             break;
        default:
             if(colum>=80)
             {
                 row++;
                 colum=0;
             }
             else
              colum++;
             if(row>24)
              scroll_up(2);
              if(console_buff_pos>=100)
               flush_buff();
            console_buff[console_buff_pos++]=((u8)ch|color<<8);
    } 
    return 0;         
    
}    


*/

/*


int flush_to_vdu(register int *str)
{
     int i=0;
     int far *v_index=(int far *)(vid_seg+row*160+colum*2);
    
     while(str[i])
     {
         *v_index++=str[i++];
         
     }
     colum+=i;
     
     if(colum>80)
     {
         row++;
         colum=(colum)%80;
     }
     
     return SUCCESS; 
}                              
                       
                               



int flush_to_vdu(char ch,int color)
{
       int i=0;
       if(ch=='\n')
       {
           row++;
           colum=0;
       }
       else if(ch=='\t')
        colum=colum+4;
       else if(ch=='\b')
       {
           if(colum>0)
            colum--;
       }           
       else
       {
       
           vid_index=(vid_seg+row*160+colum*2);
           *vid_index=ch;
            vid_index++;
           *vid_index=color;
       
            colum++;
            if(colum>79)
            {
                row++;
                colum=colum%80;
            } 
            if(row>24)
             scroll_up(1);      
                  
       }        
       return SUCCESS; 
}   

*/

int clear_screen()
{
    int byte=row*40+colum;
    long far *vid_ptr=(long far *)vid_seg;
    row=0;
    colum=0;
    while(byte)
    {
        *vid_ptr++=0;
        byte--;
    }    
    return 0;  
          
}
