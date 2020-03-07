
#include<type.h>
#include<vdu.h>
#include<stdio.h>
#include<memory.h>
#include<fs.h>
#include<vfs.h>
#include<shell.h>

// temprory headers 
#include<bios.h>
#include<conv.h>
#include<math.h>
#include<disk.h>
#include<interrupt.h>




// temprorary headers



void setvect(int num,void (*vect)())
{
    
	u16 _CS;
	u32 n=(u32)vect;
	__asm mov _CS,ds;
	
	u16 far *off=(u16 far *)(num*4);
    u16 far *seg=(u16 far *)(num*4)+1;
    *seg=_CS;
    *off=(u16)vect;
    	

}



extern "C" int kmain()
{
      
       init_vdu();
       clear_screen();
       initialize_memory();
       init_drive();
       init_filesystem();
       register_fs();
       init_shell();
       
       
       return 0;
}      
