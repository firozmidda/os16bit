

#include<type.h>
#include<fs.h>
#include<disk.h>
#include<partition.h>
#include<stdio.h>
#include<memory.h>
#include<string.h>
#include<math.h>
#include<ntfs.h>
#include<fat12.h>
#include<fat32.h>

u32 ext_add=0;
char *hdd_name_list="CDEFGHIJKLMNOPQRST";
char *fdd_name_list="AB";
int fdd_list_no=0;
int hdd_list_no=0;

PFILESYSTEM pfront=NULL,prear=NULL;

char get_hdd_name()
{
     return hdd_name_list[hdd_list_no++];
}

char get_fdd_name()
{
     if(fdd_list_no>2)
      return 'Z';
     return fdd_name_list[fdd_list_no++];
}
    


PFILESYSTEM get_fs_ptr()
{
        return pfront;            
}




int show(PFILESYSTEM ptr)
{
    
     printf("\n Filesystem type ");
     switch(ptr->fs_type)
     {
          case NTFS:
           printf(" NTFS  MFT =0x%lx ",ptr->dntfs.start_mft);
          break;
          
          case FAT12:
            printf(" FAT12  ");   
            printf(" Root Directory Sector  =%lu ",ptr->dfat.root_sec);
            printf(" Data Sector  =%lu ",ptr->dfat.data_sec);
          break;
          
          case FAT32_1:
          case FAT32_2:
            printf(" FAT32  Data sector= 0x%lx ",ptr->dfat.data_sec);
            break;
                     
          
                  
     }
     
                           
     return TRUE;
    
}    

int make_floppy_fs(_drive far *d)
{
    PFILESYSTEM temp;
    // 1 read boot sector
    u8 far *buffer=(u8 far *)farmalloc(512);
    u16 sec_per_fat,root_sz;
    u8 nr_fat;
    
    temp=(PFILESYSTEM)farmalloc(sizeof(FILESYSTEM));
    if(temp==NULL)
    {
         printf("\n Cannot Allocate  Memory");
         return 0;
    }              
    temp->path[0]=NULL;    // set path= NULL;
    temp->mount_flag=FALSE;
    temp->fs_type=FAT12;
    temp->start_lba=0;
    temp->dptr=d;
    temp->dev_name=get_fdd_name();
    temp->next=NULL;
    if(abs_read_disk(d->drive_no,0,0,1,1,buffer)!=0)
    {
           printf("\n ERROR IN READING FLOPPY FILSYSTEM WITH ABS READ");
           return FAILURE;
    }
    memcpy(&temp->byte_per_sec,buffer+BYTE_PER_SEC_OFF,2);
    memcpy(&temp->clus_sz,buffer+CLUS_SZ_OFF,1);
    memcpy(&temp->dfat.fat_sec,buffer+RES_SEC_OFF,2);
    
    temp->dfat.fat_sec+=temp->start_lba;
    
    memcpy(&nr_fat,buffer+FAT_NR_OFF,1);
    memcpy(&sec_per_fat,buffer+SEC_PER_FAT16_OFF,2);

    temp->dfat.root_sec=temp->dfat.fat_sec+(sec_per_fat*nr_fat);
    
    memcpy(&root_sz,buffer+ROOT_SIZE_OFF,2);
    
    root_sz=(root_sz*32);
    
    temp->dfat.data_sec=temp->dfat.root_sec+(root_sz/512);
    
    if(pfront==NULL)
    {
         pfront=temp;
         prear=temp;
    }
    else
    {
        prear->next=temp;
        prear=temp;
    }                             
    show(temp);
    free(buffer);
    return 0;
    
    
}    

int read_partition(_drive far *d,partition part,u32 lsec)
{
     PFILESYSTEM temp;
     u8 far *buffer=(u8 far *)farmalloc(512);
     u32 sec_per_fat;
     u8 nr_fat;
    
    // printf("\n lsec= %p type =%x ",lsec,part.parttype);
     temp=(PFILESYSTEM)farmalloc(sizeof(FILESYSTEM));
     if(temp==NULL)
      printf("\n Cannot Allocate Memory For Filesystem ");
     temp->path[0]=NULL;    // set path= NULL;
     temp->mount_flag=FALSE;
     temp->fs_type=part.parttype;
     temp->start_lba=lsec+part.partitionBegining;
     temp->dptr=d;
     temp->dev_name=get_hdd_name();
     temp->next=NULL;
     if(read_disk(d,temp->start_lba,1,buffer)!=0)
     {
            printf("\n Error Cannot Read partiton %x ",part.parttype);
            return FAILURE;
     }
     memcpy(&temp->byte_per_sec,buffer+BYTE_PER_SEC_OFF,2);
     memcpy(&temp->clus_sz,buffer+CLUS_SZ_OFF,1);
     
      switch(part.parttype)
      {
           case FAT32_1:
           case FAT32_2:
                memcpy(&temp->dfat.fat_sec,buffer+RES_SEC_OFF,2);
                temp->dfat.fat_sec+=temp->start_lba;
                memcpy(&nr_fat,buffer+FAT_NR_OFF,1);
                memcpy(&sec_per_fat,buffer+SEC_PER_FAT32_OFF,4);
                temp->dfat.data_sec=temp->dfat.fat_sec+mul32(sec_per_fat,nr_fat);
                memcpy(&temp->dfat.dir_sec,buffer+ROOT_DIR_OFF,4);
               break;
           
           case NTFS:
                memcpy(&temp->dntfs.start_mft,buffer+NTFS_MFT_OFF,4);
                break;
      }              
      if(pfront==NULL)
      {
           pfront=temp;
           prear=temp;
      }
      else
      {
          prear->next=temp;
          prear=temp;
      }                             
     show(temp);
     free(buffer);
     return true; 
}    

int make_filesystem(_drive far *d,u32 lsec)
{
    
      u32 newsec; 
     _MBR far *mbr=(_MBR far *)farmalloc(512);
     if(mbr==NULL)
     {
        printf("\n Cannot Allocate Memory For MBR  ");
        return 0;
     }              
     
     memset(mbr,0,512);
      
     int error=0;
     if(lsec==0)
     {  
          error=abs_read_disk(d->drive_no,0,0,1,1,mbr);
          if(error!=0)
          {
               printf("\n Cannot Read ABS Disk Error code =0x%x ",error);
          }     
                
     }                          
     else if((error=read_disk(d,lsec,1,mbr))!=0)
     {
        printf("\n Cannot Read Disk Error Code 0x%x ",error);
        return FAILURE;
     }
    
     for(int i=0;i<NR_PARTITIONS;i++)
     {
          if(mbr->part[i].parttype==EXTEND_1||mbr->part[i].parttype==EXTEND_2)
          {
               newsec=mbr->part[i].partitionBegining; 
               if(ext_add==0)
               {
                   ext_add=mbr->part[i].partitionBegining;
               }
               else                                                                              
               newsec=ext_add+mbr->part[i].partitionBegining;                   
               make_filesystem(d,newsec);
           }
           else
           {   
              //  printf("\n Partition  %x ",mbr->part[i].parttype );
                switch(mbr->part[i].parttype)
                {
                     case 0:
                          break;                        
                     case FAT32_1:
                     case FAT32_2:
                     case NTFS:
                        read_partition(d,mbr->part[i],lsec);    
                        break;                                    
                     default:
                       printf("\n Unknown Partition  %x",mbr->part[i].parttype );
                  }            
                      
           }     
           
     } 
     free(mbr);

     return true;   
      
    
}
int init_filesystem()
{
    pfront=NULL;
    prear=NULL;
    _drive far *d=get_drive_ptr();
    
    printf("\n\n Detecting Filesystem ..");
    while(d!=NULL)
    {
         if(d->dev_type==HDD)
          make_filesystem(d,0);
         else if(d->dev_type==FDD)
          make_floppy_fs(d);
         d=d->next;
    }
    return true;              
    
}    

int fs_mount(PFILESYSTEM ptr,char ch)
{
    
    if(ptr->mount_flag)
     return true;
    
    PENTRY root=(PENTRY)farmalloc(sizeof(ENTRY));
    
    ptr->path[0]=ch;
    ptr->path[1]=':';
    ptr->path[2]=0;
   
    strcpy(root->name,"ROOT");
    
    ptr->root_dir=root;
    ptr->cur_dir=root;
    
    ptr->mount_flag=1;
    
    root->mft=5;
    root->next=NULL;
    root->parent=NULL;
      
    fs_open_dir(ptr,root);
   
    
    return true;
    
}    

int fs_open_dir(PFILESYSTEM ptr,PENTRY entry)
{
    switch(ptr->fs_type)
    {
          case FAT12:
               entry->child=make_fat12_dtree(ptr,entry);
               break;
        
          case FAT32_1:
          case FAT32_2:
               entry->child=make_fat32_dtree(ptr,entry);
               break;                        
        
          case NTFS:
               entry->child=make_ntfs_dtree(ptr,entry);
               
               break;
          default :
               printf("\n Cannot Open Directory of Unknown Partition Type = 0x%x ",ptr->fs_type);
    }                    
    return true;
    
} 

int update_path(PFILESYSTEM fsptr,char *str,int flag)
{
      
       char far *st;
       st=fsptr->path;
       while(*st++);
       
       if(flag==ADD)
       {
            
            st--;
            
            if(*st!='\\')
            *st='\\';
            
            st++;
            
            while(*str)
             *st++=*str++;
            
            *st=0; 
                     
        }
        else if(flag==SUB)
        {
              st--;
              if(*st=='\\')
              st--;
              
              while(*st!='\\')
               st--;
              
              *st=0; 
               
        } 
        return 0;
}                               
             

int fs_open_file(PFILESYSTEM fsptr,PENTRY entry,u8 far *buffer)
{
    
    switch(fsptr->fs_type)
    {
        case FAT12:
             open_fat12_file(fsptr,entry,buffer);
             break;                  
        
        case FAT32_1:
        case FAT32_2:
             open_fat32_file(fsptr,entry,buffer);
             break;          
        
        case NTFS:
             open_ntfs_file(fsptr,entry,buffer);
             break;
        default:
                printf("Cannot Open File Of Unknown Partition Type = 0x%x ",fsptr->fs_type);
    } 
    return 0;
}                                   
