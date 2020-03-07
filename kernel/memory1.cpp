
/******************************************************************************
   mem.cpp
		-Memory manager

   arthor\ Firoz
******************************************************************************/

#include <type.h>
#include<memory.h>
#include<stdio.h>
#include<math.h>


enum {Alloc=0, rel=1};


memory mem[700];   //maximum number of address it can hold
u16 mem_pos=0;     
u16 free_pos=0,free_size=1;
u16 res_pos=0,res_size=0;


u32 mem_limit=0x80000;
u32 block_addr=0x80000;

struct block_table
{
    u32 addr;
    u32 size;
    u8 type;
}block_tab[5];
      
int block_pos=0;


void initialize_memory()
{
     mem[mem_pos].ptr=0x20000;   //starting memeory allocating address
     mem[mem_pos].size=0x70000;   // size of allocation
     mem[mem_pos].type=FREE;  
       
}      

int free_block(void far *ptr)
{
    u32 addr=(u32)ptr;
    return 0;
    
}    



u32 alloc_block(u16 size)
{
    
    u32 t_size=size;
    u32 addr=0;
    block_addr-=t_size;
    while((block_addr & 0xF)!=0)
    {
         block_addr--;
         t_size++;
    }
    
    block_tab[block_pos].addr=block_addr;
    block_tab[block_pos].size=t_size;
    block_pos++;
    addr=MK_FP(div32(block_addr,0x10),0000);
    return addr;

    
}   

// This function combine two  free memory it holds address of first and size is sum of both mem size  

void add_mem()
{
     u32 new_ptr;
     int j=0;
     for(int  i=free_pos;i<(free_pos+free_size);i++)  //the  loop ren from free pos to end of array
     {
         new_ptr=mem[i].ptr+mem[i].size;  //get the new address for searching
         j=i+1;     
        
         if(new_ptr==mem[j].ptr)
         {                                                                 
            mem[i].size=mem[i].size+mem[j].size;                    
            while(j<(free_pos+free_size))
            {
                 mem[j]=mem[j+1];
                 j++;
            }
            free_size--;    
            mem_pos--;                              
                
         }
     }   
       
}                                                                      



void sort_mem()
{
     memory temp;
     u16 pos;
     u32 small;
     
     int j;
     //seperate reserve and free 
     for(int i=1;i<=mem_pos;i++)
     {
         if((mem[i].type==RES)&&(mem[i-1].type==FREE))
         {
                                                    
            j=i-1;
            temp=mem[i];  
            while((mem[j].type==FREE)&&(j>=0))
            {
                                              
                mem[j+1]=mem[j];
                j--;
            }
            mem[j+1]=temp;
          
         } 
     }  
              
     // sort the reserve           
     for(i=res_pos+1;i<(res_pos+res_size);i++)
     {
          j=i-1;
          temp=mem[i];
          while((mem[j].ptr>temp.ptr)&&(j>=res_pos))
          {
                                            
               mem[j+1]=mem[j];
               j--;
          }
          mem[j+1]=temp;
     }                                                           
    
     // sort the free mem
     for(i=free_pos+1;i<(free_pos+free_size);i++)
     {
          j=i-1;
          temp=mem[i];
          while((mem[j].ptr>temp.ptr)&&(j>=free_pos))
          {
                                             
               mem[j+1]=mem[j];
               j--;
          }
          mem[j+1]=temp;
     }                                                           
       

                                               
}


u32 farmalloc(u16 size)
{    
    
    u32 temp_ptr,new_ptr,new_size;
    u8 flag=0;
   // printf("\n size= %l ",size); 
    for(int i=free_pos;i<(free_pos+free_size);i++)
    {
                           
       if(mem[i].size>=size)
       {                                                                                                                      
           temp_ptr=mem[i].ptr;
           new_ptr=temp_ptr+size;
           new_size=mem[i].size-size;
           //printf(" ptr=%p new ptr=%p new size =%p ",temp_ptr,new_ptr,new_size);
           mem[i].size=size;
           mem[i].type=RES;
           res_size++;
           free_pos++;
           free_size--;
           flag=1;
           break;
       }
               
         
    }
    if(flag==1)
    {
        if(new_size)
        {
           mem_pos++; 
           free_size++; 
           mem[mem_pos].ptr=new_ptr;
           mem[mem_pos].size=new_size;
           mem[mem_pos].type=FREE;
          // printf("\n mem pos= %d ",mem_pos);
        }
        else;
       //  printf(" best fit space ");
            
        sort_mem(); 
        temp_ptr=make_fp(temp_ptr);
        
        return temp_ptr;
   }
   else
   {
       printf(" no free space ");
       printf(" mem ptr= %u ",mem_pos); 
       view_table();
       return 0;
   }   
}   

void free(void far *fptr)
{
     
     u32 ptr=(u32)fptr;
     u16 flag=0;
     //printf(" free ptr =%p ",ptr);
     ptr=break_fp(ptr);
     
     for(int i=res_pos;i<=(res_pos+res_size);i++)
     {
           if((ptr==mem[i].ptr)&&(mem[i].type==RES))
           {
                // printf(" size =%p ",mem[i].size);                                   
                 mem[i].type=FREE;
                 res_size--;
                 free_size++;
                 free_pos--;
                 flag=1;
                 break;
           }
     }
     if(!flag)
     {
         printf("\n Cannot FREE %p ",ptr);
        
     }     
     
     sort_mem();
     add_mem();                                             
                                                      
}   
                
void view_table()
{
     printf("\n======DISPLAYING MEMORY TABLE======");
     printf( "res pos=%u res size=%u fee pos= %u free size =%u ",res_pos,res_size,free_pos,free_size);
     for(int i=0;i<(free_size+res_size);i++)
     {
         printf(" \n  ptr=%p size =%l type = ",mem[i].ptr,mem[i].size);
         if(mem[i].type==FREE)
          printf("FREE  ");
         else if(mem[i].type==RES)
          printf("reserve  ");
      }
      
}           
               
  
