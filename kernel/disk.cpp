
#include<type.h>
#include<disk.h>
#include<bios.h>
#include<stdio.h>
#include<disk.h>
#include<memory.h>
#include<string.h>

_drive far *dfront=NULL,far *drear=NULL;


void view_dpte(int drive,int num)
{
       
     REGS ir,or;
     ext_param_block ext_param;
     u16 far *io_addr; 
     
     memset(&ext_param,0,sizeof(ext_param));
     ext_param.b_size=0x1A;
     
     if(num & 0x02)
       ext_param.b_size=0x1E;
     else if(num & 0x04)
       ext_param.b_size=0x42;
              
     ir.h.ah=0x48;
     ir.h.dl=drive;
     ir.x.si=(u16)&ext_param;
     int86(0x13,&ir,&or);         
     if(or.x.cflag)
     {
         printf(" Cannot Read Drive parameter ");
         return ;
     }
     printf("\n Host Bus =");
     for(int i=0;i<4;i++)
      putc(ext_param.host_bus[i]);
    
     printf(" Interface type =");
     for(i=0;i<8;i++)
      putc(ext_param.interface_type[i]);
       
     io_addr=(u16 far *)(ext_param.edd);
     printf("\n Dsvice Base Address  =%x ",*io_addr);
     io_addr++;
     printf(" Device Controller Address =%x ",*io_addr);
     io_addr+=4;
     
}    



u16 get_drive_param(u16 drive,int  *sector,int *track,int *head)
{

      u8 hdrive,fdrive,drive_no=u8(drive);
	  u8 thead=0;
	  u16 h1,h2;
	  u16 ttrack=0;
     _asm
	 {
	      mov ax,00
		  int 13h
	      mov dl,drive_no;
		  mov ah,08h
		  int 13h
		  jc error
		  jmp sucess
	 };
	 
	error:
      return 0;
   sucess:
      __asm mov thead,dh
      __asm mov ttrack,cx
      __asm mov hdrive,dl
      __asm mov fdrive,bh
          
  
      h1=(ttrack&0xff00)>>8;
      h2=(ttrack&0x00c0)<<2;
	 
	  
     *track=(h1|h2)+1; 	  
	 *head=thead+1;
     *sector=(ttrack&0x3f); 
     return 1;	 
    
}



int  init_drive()
{
     _drive far *temp;
     REGS ir,or;
     dfront=NULL;
     drear=NULL;
     int t_sec,t_track,t_head;
     
     printf("\nSearching For Floppy Drives ..."); 
     for(int i=0;i<4;i++)
     {
          if(get_drive_param(i,&t_sec,&t_track,&t_head))
          {
              temp=(_drive far *)farmalloc(sizeof(_drive));
              if(temp==NULL)
              {
                   printf(" Cannot Allocate memory For Drive FDD  ");
                   return 0;
              }                
              temp->next=NULL;
              temp->drive_no=i;
              temp->head=t_head;
              temp->sector=t_sec;
              temp->track=t_track;
              temp->ext_flag=0;
              temp->dev_type=FDD;
              
              printf("\n Floppy Drive Found Number= %d ",i);
              printf(" Head =%d Cylinder =%d Sector per Cylinder =%d ",t_head,t_track,t_sec);
              
              if(dfront==NULL)
              {
                 dfront=temp;
                 drear=temp;
              }
              else
              {
                  drear->next=temp;
                  drear=temp;
              }    
                        
        }   
             
     }        
     
     // hdd 
     ir.h.ah=0x41;
     ir.x.bx=0x55aa;
     printf("\n Searching For Harddisk ...");
     
     for(i=0x80;i<0x82;i++)
     {
         ir.h.dl=i;
         int86(0x13,&ir,&or);
         if(or.x.cflag==0&&or.x.bx==0xaa55)
         {
              temp=(_drive far *)farmalloc(sizeof(_drive));
              temp->next=NULL;
              if(temp==NULL)
               printf(" Cannot Allocate memory For Harddisk "); 
              temp->ext_ver=(or.h.cl&0x07);   //masking 3 usefull bits
              temp->bitmap=or.h.ah;
              temp->drive_no=i;
              temp->ext_flag=(or.x.cx & 0x01);
              temp->dev_type=HDD;
              printf("\n Harddisk Found ");
              if(or.x.cx & 0x04)
              {
                   view_dpte(i,temp->ext_ver);
              }          
              if(dfront==NULL)
              {
                  dfront=temp;
                  drear=temp;
              }
              else
              {
                   drear->next=temp;
                   drear=temp;
                       
              }
              showdisk(temp);
         } 
     } 
     return TRUE;                      
      
}    

_drive far *get_drive_ptr()
{
      return dfront;
      
} 

void showdisk(_drive far *d)
{
        printf("\n Disk no = %x Extension Version=%x ",d->drive_no,d->ext_ver&0x07);
}          


int read_disk(_drive far *d ,u32 sector,int nsector,void far *buffer)
{
    disk_access_packet dap;
    REGS ir,or;
    
    memset(&dap,0,sizeof(dap));
    
    dap.res=0;
    dap.res2=0;
    dap.nsector=nsector;
    dap.sector[0]=sector;
    dap.sector[1]=0;
    dap.seg_off=(u32)buffer;
    dap.size=0x10;
    
    ir.h.dl=d->drive_no;
    ir.x.si=(u16)&dap;
    ir.h.ah=0x42;
    int86(0x13,&ir,&or);
    
    if(or.x.cflag!=0)
    {
        printf("\n Error Occured Cannot Read Disk ");             
        printf("\nDrive Number  =0x%x ",d->drive_no);
        printf(" Sector = 0x%lx Number of Sector =0x%x ",sector,nsector);
        printf(" Error Code =%x ",or.h.ah);             
        return (int)or.h.ah;
        
    }    
    else return 0;
    
}   

int abs_read_disk(int drive,int head,int track,int sector,int nsector,void far *buffer)
{
    
    REGS ir,or;
    SREGS s;
   
    ir.h.ah=2;
    ir.h.al=nsector;
    ir.h.ch=track;
    ir.h.cl=sector;
    ir.h.dh=head;
    ir.h.dl=drive;
    
    s.es=FP_SEG(buffer);
    ir.x.bx=FP_OFF(buffer);
    
    int86x(0x13,&ir,&or,&s);
    
     if(or.x.cflag!=0)
     {
         printf("\n======== ERROR Occured During ABS Reading Disk =============");
         printf("\n Drive No= 0x%x ",drive);
         printf(" Sector = 0x%x Number of Sector =%u ",sector,nsector);
         printf(" Error Code =0x%x ",or.h.ah);
         printf("\n=================================================");
         return (int)or.h.ah;
         
     }    
     else return 0;
    
}     

int read_floppy(_drive far *d,int lsec,int nsec,u8 far *buffer)
{
    
      int head,track,sector;
      int h,t,s;
      
      head=d->head;
      track=d->track;
      sector=d->sector;
     
      if(sector == 0 || head == 0)
         return 1;
      
      h=(lsec / sector) % head;
      t=lsec / (sector * head);
      s=(lsec % sector )+1;
      //printf(" H= %d T= %d S= %d ",h,t,s);
      return abs_read_disk(d->drive_no,h,t,s,nsec,buffer);
      
    
}    
