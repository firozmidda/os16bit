
#include<type.h>
#include<memory.h>
#include<string.h>
#include<stdio.h>

u32 max_mem_limit=0x90000;
u32 min_mem_limit=0x20000;
u32 free_mem_size= 0x70000;
u32 placement_addr=0x20000;

int mem_pos=-1;

struct memory_table
{
    int size;
    u32 ptr;
    
 }free_tab[1500];
 
 
void initialize_memory()
{
     
     max_mem_limit=0x90000;
     min_mem_limit=0x20000;
     free_mem_size=0x70000;
     placement_addr=0x20000;
       
 
}

int check_memory_mgmt()
{
   
    return true;
    
}     
 
u32 get_free_ptr(int size)
 {
                     
                     
     // this size is increment by 2 
     // so don`t confuse
     
     u32 ptr,ptr2;
     if(mem_pos==-1)
      return 0;
     
     for(int i=0;i<=mem_pos;i++)
     {         
         if((free_tab[i].size)>=size)  
         {                   
             if((free_tab[i].size)==size)
             {   
                 ptr=free_tab[i].ptr;
                 // filling the Vacant Place 
                 for(int j=i;j<mem_pos;j++)
                   free_tab[j]=free_tab[j+1];
                 // decrement the Free memory Size  
                 mem_pos--;
                 
                 ptr=make_fp(ptr);
                 // decrement and Write To Memory 
                 size=size-2;
                 memcpy((void far *)ptr,&size,2);
                 ptr2=make_fp(ptr+2);
                 
                 return (ptr2);
                     
             }       
         
             else
             {
                
                 
                 ptr=free_tab[i].ptr;
                
                 free_tab[i].ptr+=size; // increase ptr by size to get a new ptr;
              
                 free_tab[i].size-=size;  // decrease the Size of left memory
                
                 ptr=make_fp(ptr);
                 // decrease size for Writing it to memory
                 size=size-2; 
                 memcpy((void far *)ptr,&size,2);
                 ptr2=make_fp(ptr+2);
                
                 return (ptr2);     
                  
             }
         
        }
        
     }
        
 
     return 0;
 }
 
 
 int  add_memtab(void far *ptr,int size)
 {
    
      u32 p=(u32)ptr;
       printf(" comes mem= %p ",p);
      p=break_fp(p);
     // if(mem_pos==1499)
     //  printf(" \n Memory Full ");
      
     
      for(int i=0;i<=mem_pos;i++)
      {
            if(size<free_tab[i].size)
            {
                  for(int j=mem_pos+1;j>i;j--)
                  {
                      free_tab[j]=free_tab[j-1];
                  }
                  free_tab[i].size=size;
                  free_tab[i].ptr=p;
                  mem_pos++;
                  return 0;
                   
            }
      }
      if(mem_pos==-1)
      {
          mem_pos++;
          free_tab[mem_pos].ptr=p;
          free_tab[mem_pos].size=size;
      }
      else
      {
          free_tab[mem_pos].ptr=p;
          free_tab[mem_pos].size=size;
          mem_pos++;
      }    
           
      return 0;
 
 }
 
 
u32 farmalloc(u16 size)
{
      u32 ptr,ptr2,sz;
      size=size+8;
      u16 tsize=size+2;
      u16 cur_sz;

        
      if(size<=4)
      printf("\n Error Caught ");

      ptr=get_free_ptr(tsize);
      
      if(!ptr)
      {
            if(placement_addr+tsize<=max_mem_limit)
            {   
                 ptr=placement_addr;
                 ptr2=make_fp(ptr);
                 
                 sz=0x10000-(u16)ptr2;
                 if(sz<tsize)
                 {
                    cur_sz=u16(sz);         
                    // cur_sz=+1;      // 0xffff+1= 0x10000   occupiying last 1 bit
                    
                     printf("Override occur %u %x",cur_sz,(u16)ptr2);        
                     free_mem_size-=cur_sz;       // subtract free memory            
                     cur_sz-=2;                   
                     memcpy((void far *)ptr2,&cur_sz,2);
                     ptr2=make_fp(ptr);
                     free((void far *)ptr2);       // free  the memory for table
                     placement_addr+=sz;                     
                     ptr=placement_addr;
                     ptr2=make_fp(ptr);
                     printf(" Offset=%x ",(u16)ptr2);
                     
                 }
                 free_mem_size-=tsize;
                 memcpy((void far *)ptr2,&size,2);
                 ptr2=make_fp(ptr+2);
                 placement_addr+=tsize;
                                       
                 printf("p=%p ",ptr2);
                 return ptr2;
             } 
             else
             {
               // printf( " zero =%d ",size );
                return 0;
             }   
        }
       
        return ptr;
 }
 
 
 void free(void far *ptr)
 {
      char far *cptr=(char far *)ptr;
      int size=0;
      cptr-=2;
      memcpy(&size,cptr,2);
      
      printf(" \n size =%d ptr= %p ",size,ptr);
     
      add_memtab((void far *)cptr,size+2);
     
      
 }
  
  void view_table()
  {
     for(int i=0;i<=mem_pos;i++)
     {
        ;//  printf("\n size =%d ptr= %lx ",free_tab[i].size-2,free_tab[i].ptr);
     }
  }            
                     
