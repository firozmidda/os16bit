

#include<type.h>

#ifndef _FAT_H_
 #define _FAT_H_
 #include<fs.h>

struct fat_bs
{
    u8 jmp[3];
    u8 system_id[8];
    u16 byte_per_sec;
    u8 sec_per_clus;
    u16 res_sec;
    u8 fat_copies;
    u16 root_entries;
    u16 total_sec;
    u8 media;
    u16 sec_per_fat;
    u16 sec_per_track;
    u16 head_per_side;
    u32 hidden_sec;
    u32 huge_sec;
};
struct fat_bs16
{
   u8 drive_no;
   u8 reserved;
   u8 boot_sig;
   u32 volume_id;
   u8 volume_label[11];
   u8 fat_type_label[8];
};
struct fat_bs32
{
   u32 sec_per_fat32;
   u16 flag;
   u16 fat_ver;
   u32 root_cluster;
   u16 fat_info;
   u16 backup_sector;
   u8 reserve_0[12];
   u8 drive_no;
   u8 reserve_1;
   u8 boot_signture;
   u32 volume_id;
   u8 volume_label[11];
   u8 fat_type_label[8];
};  
struct _FAT
{
    fat_bs fat12;
    union 
    {
      fat_bs16 fat16;
      fat_bs32 fat32;
    };   
};



struct ROOT_ENTRY 
{
   u8 name[8];
   u8 ext[3];
   u8 attrib;
   u8 reserve;
   u8 creat_time;
   u16 time;
   u16 date;
   u16 last_access;
   u16 cluster_hi;
   u16 last_modify_t;
   u16 last_modify_d;
   u16 cluster_lo;
   u32 size;
	
};

PENTRY make_fat_tree(PFILESYSTEM ptr,PENTRY parent);

#endif
