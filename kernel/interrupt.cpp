
#include<type.h>
#include<stdio.h>
#include<bios.h>
#include<interrupt.h>
#include<memory.h>
#include<vfs.h>
#include<string.h>
#include<vdu.h>

REGS ireg;
SREGS sreg;



int video_call(REGS ir,SREGS s)
{
      
     switch (ir.h.al)
     {
          case 0:
               //clear_screen();
               break;
     }             
     printf(" Video Call ");
     return 0;       
       
} 
 

int graphics_call(REGS ir,SREGS s)
{
     printf(" Graphics Call ");
     return 0;
}
      
int dir_mgmt_call(REGS ir, SREGS s)
{
   
    u16 DS=s.ds;
    char  path[60];
    char far *str=(char far *)MK_FP(DS,ir.x.si);
               
    switch(ir.h.al)
    {
        
        case 0:
              printf(" %c ",ir.h.bh);
              return !(vfs_mount(ir.h.bl));
             break;
             
        case 1:
             strcpy(path,str);
             return !(vfs_chdir(path));
         break;
         
         case 2:
              strcpy(path,str);
              return !(vfs_view_dir(path));
                
         break;
        
        case 4: 
             strcpy(str,get_path());
             return 0;
        break;   
        
        case 5:
             vfs_close_dir();
             return 0;
        break;  
          
          default :
                  printf(" Wrong Value ");
                               
     }
    return 0;
    
}   

int file_mgmt_call(REGS ir,SREGS s)
{
    
    switch(ir.h.al)
    {
        case 1:
             printf(" OPEN FILE ");
             break;
        case 2:
             printf(" close file ");
             break;
        case 3:
             printf(" read_file ");
             break;
   }                               
    
    printf(" FILE MANAGEMENT ");
    return 0;
}

int process_call(REGS ir,SREGS s)
{
    printf(" Proces Call ");
    return 0;
}


int memory_call(REGS ir,SREGS s)
{
    printf(" MEMORY CAll ");
    return 0;
}         



extern "C" void interrupt_31h(u16 _DS)
{
    
     u16 retval;
     _asm 
     {
         mov retval,ax
         mov ax,0x50
         mov ds,ax
      }     
      ireg.x.ax=retval;
      sreg.ds=_DS;
     _asm
     {
          mov ireg.h.bl,bl
          mov ireg.h.bh,bh
          
          mov ireg.x.cx,cx
          mov ireg.x.dx,dx
          mov ireg.x.si,si
          mov ireg.x.di,di
     };
     printf(" ax =%x ",ireg.x.ax);
     printf(" bx =%x ",ireg.x.bx);
     printf(" cx= %x ",ireg.x.cx);
     printf(" dx =%x ",ireg.x.dx);
     printf(" di =%x ",ireg.x.di);
     printf(" si =%x ",ireg.x.si);
     
     
       
                     
     switch(ireg.h.ah)
      {
            
            case 1:
                retval=video_call(ireg,sreg);   break;
            case 2:
                 retval=graphics_call(ireg,sreg); break;
            case 3:
                 retval=dir_mgmt_call(ireg,sreg); break;
            case 4:
                 retval=file_mgmt_call(ireg,sreg); break;
            case 5:
                 retval=process_call(ireg,sreg);   break;
            case 6:
                 retval=memory_call(ireg,sreg);   break;                            
            default:
                    printf(" WRONG AH VALUE ");             
                     
      }              
    
    
    
    _asm
    {
        mov ds,_DS
     };
    
       
}     


/*
extern "C" void interrupt_31h()
{
   
    REGS ireg;
    u16 retval=0x05;
    SREGS sreg;

    // save all registers
     _asm
     {
         push ax
         mov sreg.ds,ds
         mov ax,0x50
         mov ds,ax
         pop ax
         
         mov ireg.x.ax,ax
         mov ireg.x.bx,bx
         mov ireg.x.cx,cx
         mov ireg.x.dx,dx
        
         
      };
      printf(" t=%x ",0x05); 
   
      switch(ireg.h.ah)
      {
            
            case 1:
                retval=video_call(ireg,sreg);   break;
            case 2:
                 retval=graphics_call(ireg,sreg); break;
            case 3:
                 retval=dir_mgmt_call(ireg,sreg); break;
            case 4:
                 retval=file_mgmt_call(ireg,sreg); break;
            case 5:
                 retval=process_call(ireg,sreg);   break;
            case 6:
                 retval=memory_call(ireg,sreg);   break;                            
            default:
                    printf(" WRONG AH VALUE ");             
                     
      }               
      
      // restore register 
      _asm{
           
            mov ax,sreg.ds
            mov ds,ax
            mov ax,retval
           
           }; 
     
}    

*/
