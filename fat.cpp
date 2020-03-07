

#include<type.h>
#include<stdio.h>
#include<math.h>
#include<disk.h>
#include<fat.h>
#include<memory.h>
#include<string.h>


struct FAT
{
     u32 cluster;
     FAT far *next;
};

FAT far *read_fat(u32 cluster,PFILESYSTEM ptr)
{
        
        u32 fat_sec,index,lsec,temp_lsec=0,value;
        u16 pos,ecode;
        
        FAT far *ffront=NULL,far *frear=NULL,far *temp=NULL;
        _drive far *d=ptr->dptr;
        u8 far *buffer=(u8 far *)farmalloc(d->byte_per_sec);
        
        
        if(cluster<2)
         return 0;
         
        fat_sec=ptr->fat_info.fat_sec;
        
        while(cluster!=0x0fffffff)
        {
              temp=(FAT far *)farmalloc(sizeof(FAT));
              if(temp==NULL)
               printf(" cannot Allocate Memory ");
              
              temp->next=NULL;
              temp->cluster=cluster;
              printf(" cluster chain =%lx ",temp->cluster);
              if(ffront==NULL)
              {
                   ffront=temp;
                   frear=temp;
              }
              else
              {
                  frear->next=temp;
                  frear=temp;
              }                    
              index=mul32(cluster,4);
                          
              lsec=div32(index,d->byte_per_sec);
            
              lsec=lsec+fat_sec;
           
              
              if(lsec!=temp_lsec)
              {   
                   ecode=readdrive(d,lsec,1,buffer);  
                   
                   if(ecode!=0)
                   {
                         printf("Error While reading _drive sector =%lx %x ",lsec,ecode);
                         return ffront;
                   }
              }
              temp_lsec=lsec;
              pos=mod32(index,d->byte_per_sec);
              memcpy(&value,buffer+pos,4);
              cluster=value;
              
       } 
       return ffront;
             
}              
    


PENTRY make_fat_dtree(PFILESYSTEM ptr,PENTRY parent)
{
      
      FAT far *fat_ptr,far *temp;
      ROOT_ENTRY root_t;
      
      printf("\n Ptr Loaction = %lx ",ptr->drive_ptr);
      PENTRY efront=NULL,erear=NULL,etemp=NULL;
      u8 far *buffer,ecode;
      
      u32 cluster,data_sec,lsec;
      u16 total_entry;
      
      if(parent==NULL)
         cluster=ptr->fat_info.dir_clus;
      else
        cluster=parent->cluster;          
      
      
      data_sec=ptr->fat_info.data_sec;
      printf(" \n cluster =%lu data sector =%lu ",cluster,data_sec);
      total_entry=(ptr->sec_per_clus*512)/32;
      
      fat_ptr=read_fat(cluster,ptr);
      buffer =(u8 far *)farmalloc(ptr->sec_per_clus*512);
      
      while(fat_ptr!=NULL)
      {
           temp=fat_ptr;               
           lsec=data_sec+mul32((temp->cluster-2),ptr->sec_per_clus);
           printf("\n========= reading  sector %lu %lx ",lsec,temp->cluster);
          
           ecode=readdrive(ptr->drive_ptr,lsec,ptr->sec_per_clus,buffer);
           if(ecode!=0)
            printf(" Error While Reading _drive code= %x ",ecode);
            
           printf(" total entries %u ",total_entry); 
         
           for(int i=0;i<total_entry;i++)
           {
                 memcpy(&root_t,buffer+(i*32),32);
                 if(root_t.name[0]==0x0e5)
                  continue;
                 if(root_t.name[0]==0)
                  break;
                
                
                int k=0;
                etemp=(PENTRY)farmalloc(sizeof(ENTRY));
               
                if(etemp==NULL)
                {
                      printf(" cannot Allocate Memory ");
                }   
                 if(((u32)etemp<0x3000002)&&((u32)etemp>0x3000000-2))
                printf("\n*********************************** found bug ");             
                for(int j=0;j<8;j++)
                {
                     if(root_t.name[j]==' ')
                      break;
                     etemp->name[k++]=root_t.name[j];
                     
                }
                etemp->name[k]=0;
                
                if((root_t.attrib&0x10)==0)
                 etemp->name[k++]='.';
                for(j=0;j<3;j++)
                {
                     if(root_t.ext[j]==' ')
                      break;
                     etemp->name[k++]=root_t.ext[j];
                }                                
                etemp->name[k]=0;
                
                etemp->type=root_t.attrib;
                etemp->size=root_t.size;
                etemp->clus[0]=root_t.cluster_lo;
                etemp->clus[1]=root_t.cluster_hi;
                etemp->next=NULL;
                
                if(efront==NULL)
                {
                    efront=etemp;
                    erear=etemp;
                }
                else
                {
                    erear->next=etemp;
                    erear=etemp;
                }                         
                 
           }
           fat_ptr=fat_ptr->next;
           free(temp);
      }                        
      return efront;      
      
}      
PFILE_BUFFER open_fat_file(PFILESYSTEM ptr,PENTRY parent)
{
             return 0;
}            


