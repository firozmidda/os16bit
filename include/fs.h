#include<type.h>

#ifndef _FS_H_
 #define _FS_H_
 
 #include<disk.h>
 
 #define ADD 0x01
 #define SUB 0x02

struct FAT_DATA
{
    u32 fat_sec;
    u32 dir_sec;
    u32 data_sec;
    u32 root_sec;
    
};



struct NTFS_DATA
{
    u32 start_mft;
};        


struct ENTRY
{
      char name[25];
      char type;
     
      u32 size;
      
      union
      {
          u32 cluster;
          u16 clus[2];
          u32 mft;
      }; 
         
      ENTRY far *next;
      ENTRY far *child;
      ENTRY far *parent;
};
       
typedef ENTRY far *PENTRY;
     

 
struct  FILESYSTEM 
{
     char path[80];
     char dev_name;
     int mount_flag;
     int fs_type;
     u32 start_lba;
     int byte_per_sec;
     int clus_sz;
     _drive far *dptr;
     PENTRY root_dir,cur_dir;
     union
     {
           FAT_DATA dfat;
           NTFS_DATA dntfs;
     };
     FILESYSTEM far *next;
                    
        
};
typedef FILESYSTEM far *PFILESYSTEM; 
 int init_filesystem();
 PFILESYSTEM get_fs_ptr();
 int fs_mount(PFILESYSTEM ptr,char ch);
 
 int fs_open_dir(PFILESYSTEM ptr,PENTRY entry);
 int fs_open_file(PFILESYSTEM fsptr,PENTRY entry,u8 far *buffer);
 
 int update_path(PFILESYSTEM fsptr,char *str,int flag);
 
 
 
#endif 
 
 
 
