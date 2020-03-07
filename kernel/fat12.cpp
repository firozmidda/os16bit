
#include<type.h>
#include<fat.h>
#include<fs.h>
#include<disk.h>
#include<memory.h>
#include<stdio.h>

PFILESYSTEM fsys;
PENTRY fat12front=NULL,fat12rear=NULL;


struct CLUSTER12_CHAIN
{
   int cluster;
   CLUSTER12_CHAIN far *next;
   
};


CLUSTER12_CHAIN far *read_fat12(_drive far *d,int cluster)
{
       
       CLUSTER12_CHAIN far *front=NULL ,far *rear=NULL,far *temp;
       int clus = cluster;
       int lsec=1,tsec=0,index,offset,fat_nr;
       int value12,fat;
       u8 far *buffer = (u8 far *)farmalloc(512);
       if(buffer==NULL)
       {
            printf(" Cannot Allocate buffer ");
            return NULL;
       }                
       u16 far *entry;
       
       while(clus >= 2)
       {     
             
            if(clus == 0xfff) break;
            
                   
            temp = (CLUSTER12_CHAIN far *)farmalloc(sizeof(CLUSTER12_CHAIN));
            if(temp==NULL)
            {
                 printf(" cannot Allocate memory ");
            }                
            temp->next = NULL;
            temp->cluster = clus;
            printf(" Cluster = ");
            
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
            printf(" Making Cahin");         
            fat_nr = (clus * 3) / 2;
            index = fat_nr / 512;
            
            lsec = fsys->dfat.fat_sec + index;
         
            
            if(lsec != tsec)
            {
                 printf(" reading fat ");   
                 if(read_floppy(d,lsec,1,buffer)!=0)
                  printf(" Error During Reading Fat ");
            }
            tsec = lsec;
            offset = fat_nr % 512;
          
            
            entry=(u16 far *)(buffer + offset);
            if(clus & 0x00001)
              value12 = (*entry)>> 4;
            else 
              value12 = (*entry )& 0x0fff;
              
             clus=value12; 
       }  
       printf(" Comes Out ");
       free(buffer);
        return front;    
            
              
}               

int read_cluster12(int cluster,u8 far *buffer)
{
    
    int sector,clus;
    clus=cluster-2;
    sector=(clus * fsys->clus_sz)+fsys->dfat.data_sec;
   
    if(read_floppy(fsys->dptr,sector,1,buffer)!=0)
     printf(" Error During Reading Fat ");
     return 0;
    
}    

int parse_dir(PENTRY parent,u8 far *buffer,int size)
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
         if(temp==NULL)
         {
              printf(" Cannot Allocate Memory ");
         }                  
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
         
         if(fat12front==NULL)
         {
             fat12front=temp;
             fat12rear=temp;
         }
         else
         {
             fat12rear->next=temp;
             fat12rear=temp;
         }                     
         
         entry_ptr++;      
            
    }       
    return 1;
    
}     

u32 copy_cluster12(u8 far *dest,u8 far *src,u32 tot_size,u32 copied_sz)
{
    
    int tot_byte=(fsys->clus_sz * fsys->byte_per_sec);
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

int open_fat12_file(PFILESYSTEM ptr,PENTRY entry,u8 far *buffer)
{
    
    int cluster,tot_byte;
    CLUSTER12_CHAIN far *clus_chain,far *temp_chain;
    u8 far *buff;
    u32 size=0,copy_size=0;
    
    if(entry->parent==NULL)
    return 0;
    cluster=entry->cluster;
    fsys=ptr;
    
    printf(" buffer =%p ",buffer);
    clus_chain = read_fat12(ptr->dptr,cluster);
    
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
          read_cluster12(clus_chain->cluster,buff);
           
          copy_size=copy_cluster12(buffer,buff,size,copy_size);
          printf(" copied_size =%p ",copy_size);
          temp_chain=clus_chain;
          clus_chain=clus_chain->next;
          free(temp_chain);
        
    }
    free(buff);
    return 0;
    
}    

PENTRY make_fat12_dtree(PFILESYSTEM ptr,PENTRY entry)
{
       
   int cluster = 2;  
  
   u8 far *buffer;
   int tot_byte=0;
   CLUSTER12_CHAIN far *clus_chain,far *temp_chain;
   
    fsys = ptr; 
    if(entry->parent == NULL)
       cluster = 0;
    else
      cluster = entry->cluster;
 
    fat12front=NULL;
    fat12rear=NULL;
    tot_byte=(ptr->clus_sz * ptr->byte_per_sec);
    buffer=(u8 far *)farmalloc(tot_byte);
    if(buffer==NULL)
    {
         printf(" Cannot Allocate buffer ");
         return NULL;
    }                
    
    if(!cluster) // it must be root dir 
    {
         int sector=(int)ptr->dfat.root_sec;
                 
         while(1)
         {
             if(read_floppy(ptr->dptr,sector,1,buffer)!=0)
                 printf(" Error During Reading Fat ");  
             
             if(!parse_dir(entry,buffer,1))
               break;
               
              sector++; 
         }      
                  
                 
    }  
    else  // it is a sub directory
    {          
         
         clus_chain = read_fat12(ptr->dptr,cluster);
         
         buffer=(u8 far *)farmalloc(tot_byte);
        
         
         while(clus_chain != NULL)
         {
              read_cluster12(clus_chain->cluster,buffer);
              if(!parse_dir(entry,buffer,1))
               break;
              temp_chain=clus_chain;
              clus_chain=clus_chain->next;
              free(temp_chain);
                 
         }
    }                           
    free(buffer);  
    return fat12front;                          
       
       
}      
      
