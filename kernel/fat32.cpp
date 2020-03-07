
#include<type.h>
#include<fat.h>
#include<fs.h>
#include<disk.h>
#include<memory.h>
#include<stdio.h>
#include<math.h>

PFILESYSTEM fsys32;
PENTRY fat32front=NULL,fat32rear=NULL;


struct CLUSTER32_CHAIN
{
   u32 cluster;
   CLUSTER32_CHAIN far *next;
   
};


CLUSTER32_CHAIN far *read_fat32(_drive far *d,u32 cluster)
{
       
       CLUSTER32_CHAIN far *front=NULL ,far *rear=NULL,far *temp;
       u32 clus = cluster;
       u32 lsec=1,tsec=0,index,offset,fat_nr;
       u32 value32,fat;
       u8 far *buffer = (u8 far *)farmalloc(512);
       u32 far *entry;
      
       while(clus >= 2)
       {     
             
            if(clus == 0x0FFFFFFF) break;
                   
            temp = (CLUSTER32_CHAIN far *)farmalloc(sizeof(CLUSTER32_CHAIN));
            temp->next = NULL;
            temp->cluster = clus;
            
            if(front == NULL)
            {
                front = temp;
                rear = temp;
            }
            else
            {
                rear->next = temp;
                rear = temp;
            }                            
                    
            fat_nr = mul32(clus,4);
            
            index = div32(fat_nr,512);         
            
            lsec = fsys32->dfat.fat_sec + index;
         
            if(lsec != tsec)
            {
                  // read disk will be here   
                if(read_disk(d,lsec,1,buffer)!=0) //if(read_floppy!=0)
                  printf(" Error During Reading Fat ");
            }
            tsec = lsec;
            offset = mod32(fat_nr,512);
            
            
            entry=(u32 far *)(buffer + offset);
           
            clus=*entry; 
            
       }  
       
       free(buffer);
        return front;    
            
              
}               

int read_cluster32(u32 cluster,u8 far *buffer)
{
    u32 sector,clus;
    clus=cluster-2;
    sector=mul32(clus,fsys32->clus_sz)+fsys32->dfat.data_sec;
   
    if(read_disk(fsys32->dptr,sector,fsys32->clus_sz,buffer)!=0)
     printf(" Error During Reading Fat ");
     return 0;
    
}    

int parse_dir32(PENTRY parent,u8 far *buffer,int size)
{
   
    ROOT_ENTRY far *entry_ptr;
    entry_ptr=(ROOT_ENTRY far *)buffer;
    
    PENTRY temp;
    int i=0;
    for(int count=0;count<(size*16);count++)
    {
          
         if(entry_ptr->name[0]==0)
          return 0;
         
      
         if(entry_ptr->name[0]==0xe5)
         {
             entry_ptr++;                        
             continue;
         }
                  
         if(entry_ptr->attrib==0xff)
         {
              entry_ptr++;                      
              continue;
         }    
         
         
         temp=(PENTRY)farmalloc(sizeof(ENTRY));
         for(i=0;i<8;i++)
         {
              if(entry_ptr->name[i]==' ')
               break;           
              temp->name[i]=entry_ptr->name[i];
             
         } 
          
         if(entry_ptr->ext[0]!=' ')
         {
                             
             temp->name[i]='.';
             i++;
             for(int j=0;j<3;j++)
             {
                 if(entry_ptr->ext[j]==' ')
                  break;
                 temp->name[i]=entry_ptr->ext[j];
                 i++;
              }                           
         }
         temp->name[i]=0;
         
         temp->type=entry_ptr->attrib;
         temp->size=entry_ptr->size;
         temp->clus[0]=entry_ptr->cluster_lo;
         temp->clus[1]=entry_ptr->cluster_hi;
         temp->next=NULL;
         temp->child=NULL;
         temp->parent=parent;
         
         if(fat32front==NULL)
         {
             fat32front=temp;
             fat32rear=temp;
         }
         else
         {
             fat32rear->next=temp;
             fat32rear=temp;
         }                     
         
         entry_ptr++;      
            
    }       
    return 1;
    
}     



PENTRY make_fat32_dtree(PFILESYSTEM ptr,PENTRY entry)
{
       
   u32 cluster;  
   u8 far *buffer;
   int tot_byte=0;
   
   CLUSTER32_CHAIN far *clus_chain;
   
    fsys32 = ptr; 
   
    if(entry->parent == NULL)
       cluster = 2;
    else
      cluster = entry->cluster;
 
    fat32front=NULL;
    fat32rear=NULL;
    tot_byte=(ptr->clus_sz * ptr->byte_per_sec);
    buffer=(u8 far *)farmalloc(tot_byte);
    
    clus_chain = read_fat32(ptr->dptr,cluster);
    buffer=(u8 far *)farmalloc(tot_byte);
    while(clus_chain != NULL)
    {
         read_cluster32(clus_chain->cluster,buffer);
         if(!parse_dir32(entry,buffer,1))
          break;
         clus_chain=clus_chain->next;
        
    }
      
    return fat32front;                          
       
       
}      

u32 copy_cluster32(u8 far *dest,u8 far *src,u32 tot_size,u32 copied_sz)
{
    
    u16 tot_byte=(fsys32->clus_sz * fsys32->byte_per_sec);
    u32 remain=tot_size-copied_sz;
    u32 size;
    
    if(remain<tot_byte)
      size=remain;
     else
      size=tot_byte;
    
     printf(" size = %p",size); 
     for(int i=0;i<size;i++)
     {
          dest[copied_sz]=src[i];
          putc(src[i]);
          copied_sz++;
     }            
     return copied_sz; 
    
}    


int open_fat32_file(PFILESYSTEM ptr,PENTRY entry,u8 far *buffer)
{
    
    u32 cluster,tot_byte;
    CLUSTER32_CHAIN far *clus_chain,far *temp_chain;
    u8 far *buff;
    u32 size=0,copy_size=0;
    
    if(entry->parent==NULL)
    return 0;
    cluster=entry->cluster;
    fsys32=ptr;
    
    printf(" buffer =%p ",buffer);
    clus_chain = read_fat32(ptr->dptr,cluster);
    
    tot_byte=(ptr->clus_sz * ptr->byte_per_sec);
    buff=(u8 far *)farmalloc(tot_byte);
    if(buff==NULL)
    {
         printf(" cannot AlloCate memory ");
         return 0;
    }     
    size=entry->size;
    printf(" size 2 =%p ",size);
    while(clus_chain != NULL)
    {
          read_cluster32(clus_chain->cluster,buff);
           
          copy_size=copy_cluster32(buffer,buff,size,copy_size);
          printf(" copied_size =%p ",copy_size);
          temp_chain=clus_chain;
          clus_chain=clus_chain->next;
          free(temp_chain);
        
    }
    free(buff);
    return 0;
    
}    
      
