

#include<type.h>

#ifndef _MEMORY_H_
 #define _MEMORY_H_
 
 
enum {FREE=1,RES=2};

struct memory
{
     u32 ptr;
     u32 size;
     u8 type;
};
extern "C" u32 make_fp(u32);
extern "C" u32 break_fp(u32);
extern "C" u32 mk_fp(u16,u16);
extern "C" u16 fp_seg(u32);
  

#define MK_FP(seg,off) (mk_fp(seg,off))
#define FP_SEG(fp)     (fp_seg((u32)fp))
#define FP_OFF(fp)     ((u16)fp) 


int check_memory_mgmt();

void initialize_memory();
u32 farmalloc(u16 size);
void view_table();   
void free(void far *fptr);   
u32 alloc_block(u16 size);

 
 
 
 #endif
