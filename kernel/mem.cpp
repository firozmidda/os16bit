#include <type.h>
#include<memory.h>
#include<stdio.h>
#include<math.h>

#define BLOCK_SZ 16

struct memory_table
{
       u32 addr;
       u32 size;
}free_tab[500],res_tab[1500];
int free_pos=0;
int res_pos=0;

int save_num=0;



void initialize_memory()
{
    free_tab[0].addr=0x40000;
    free_tab[0].size=0x50000;
    free_pos++;
    
}


int make_far(u32 *p)
{
    u32 ptr=0;
	u16 seg=div32(*p,0x10);
    u16 off=0;
	ptr=MK_FP(seg,off);
	*p=ptr;
	return 0;
	
}

int make_near(u32 *p)
{
    u32 ptr;
	u16 seg=FP_SEG(*p);
	ptr=mul32(seg,0x10);
	*p=ptr;
	return 0;

}


int addition_of_mem()
{
   int n=0;  
   for(int c=0;c<2;c++)
   {
       if(save_num+1>=free_pos)
         save_num=0;
       
        n=free_tab[save_num].addr+free_tab[save_num].size;
        
        if(n==free_tab[save_num+1].addr)
        {       
             free_tab[save_num].size+=free_tab[save_num+1].size;
             for(int j=save_num+1;j<free_pos;j++)
              free_tab[j]=free_tab[j+1];
              
             free_pos--; 
             
                                       
        }
        save_num++;                              
           
            
   }   
   return 0;      
}    

void view_table()
{
    
    addition_of_mem();
    printf("\n ===============Content of MEM TAB====================\n ");
    for(int i=free_pos-1;i>=0;i--)
    {
         printf(" \n FRE Addr =%lx size =%lx ",free_tab[i].addr,free_tab[i].size);
    }
    printf(" \n");
    for(i=res_pos-1;i>=0;i--)
    {
         printf(" \n RES Addr =%lx size =%lx ",res_tab[i].addr,res_tab[i].size);
    }
    printf("\n ============================================\n ");
 
    
    
}   
u32 get_free_mem(u32 size)
{
    int done=0;
    u32 ptr=0;
    for(int i=0;i<free_pos;i++)
    {
           if(free_tab[i].size==size)
           {
		        
                done=1;
                ptr=free_tab[i].addr;
                for(int j=i;j<free_pos;j++)
                 free_tab[j]=free_tab[j+1];
                free_pos--;
                break;
           }
           else if(free_tab[i].size>size)
           {
              
               done=1;
               ptr=free_tab[i].addr;
               free_tab[i].addr+=size;
               free_tab[i].size-=size;
               break;
           }                             
    }
    if(!done)
       return 0;
   
    return ptr;         
    
}


int add_free_mem(u32 ptr,u32 size)
{
   
    u32 n=ptr-size;
    
    int done=0,found=0,pos=0;
    for(int i=0;i<free_pos;i++)
    {
         if(free_tab[i].addr==n)
         {
             done=1;
             free_tab[i].size+=size;
             break;
         }
         else if(ptr<free_tab[i].addr)
         {
              pos=i;
              found=1;
              break;
         }
    }
    
    if(!done)
    {
        if(found)
        {
            
             for(int j=free_pos;j>pos;j--)
              free_tab[j]=free_tab[j-1];
             
             free_tab[pos].addr=ptr;
             free_tab[pos].size=size;
        }
        else
        {
           
            free_tab[free_pos].addr=ptr;
            free_tab[free_pos].size=size;
            
        }
        free_pos++;
    }
    addition_of_mem();
    return 0;
           
                
}                                                   



int add_res_mem(u32 ptr,u32 size)
{
    
    int found=0;
    int pos=0;
    for(int i=0;i<res_pos;i++)
    {
         if(ptr<res_tab[i].addr)
         {
               pos=i;
               found=1;
               break;
         }
   }
   if(found)
   {
         for(int j=res_pos;j>pos;j--)
           res_tab[j]=res_tab[j-1];
         
         res_tab[pos].addr=ptr;
         res_tab[pos].size=size;
         res_pos++;
       
   }
   else
   {
        res_tab[res_pos].addr=ptr;
        res_tab[res_pos].size=size;
        res_pos++;
       
   }
   
   return 0;
}                                        
    
u32 get_res_mem(u32 ptr)
{
    u32 size=0;
    int found=0;
    int pos=0;
    for(int i=0;i<res_pos;i++)
    {

        if(res_tab[i].addr==ptr)
        {
             found=1;
             pos=i;
             size=res_tab[i].size;
             break;
                           
        }                       
    }
    if(found)
    {
         for(int i=pos;i<res_pos;i++)
          res_tab[i]=res_tab[i+1];
         
         res_pos--;
         
    }
             
    return size;
}    

u32 farmalloc(u16 tsize)
{
    u32 size=0;
	u32 ptr;
   
	
    int n=tsize/BLOCK_SZ;
    if(tsize%BLOCK_SZ)
     n++;
    
    size=n*BLOCK_SZ; 
    
	ptr=get_free_mem(size);
    
	if(ptr)
    {
	   
       add_res_mem(ptr,size);
	   make_far(&ptr);
	   
    }
	
    return ptr;
    
    
} 
void free(void far *p)
{
    u32 ptr=(u32)p; 
	make_near(&ptr);
    u16 size=get_res_mem(ptr);
    if(size)
    {
        add_free_mem(ptr,size);
    }
   
}      

u32 alloc_block(u16 block)
{
     return farmalloc(block);
}

      
    
