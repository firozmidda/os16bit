
#include<type.h>

#ifndef _DISK_H_
 #define _DISK_H_
 
 enum { FDD=1,HDD=2};
 
struct disk_access_packet
{
      u8 size;
      u8 res;
      u8 nsector;
      u8 res2;
      u32 seg_off;
      u32 sector[2];
};  


struct ext_param_block
{
    u16 b_size;
    u16 info_flag;
    u32 t_cyl;
    u32 t_head;
    u32 t_sec;
    u32 sec[2];
    u16 byte_per_sec;
    // v 2.0 +
    u32 edd;
    //v3.0+
    u16 signature;
    u8 length;
    u8 res[3];
    u8 host_bus[4];
    u8 interface_type[4];
    u8 interface_path[8];
    u8 device_path[8];
    u8 res1;
    u8 checksum; 
};
     

struct _drive
{
    int ext_flag;
    int ext_ver;
    int bitmap;
    int head;
    int track;
    int sector;
    int drive_no;
    int dev_type; 
    _drive far *next;
    
};       
 
 
 int  init_drive();
 _drive far *get_drive_ptr();
 int read_disk(_drive far *d,u32 sector,int nector,void far *buffer);
 void showdisk(_drive far *d);
 int abs_read_disk(int drive,int head,int track,int sector,int nsector,void far *buffer);
 int read_floppy(_drive far *d,int lsec,int nsec,u8 far *buffer);
 
 
 #endif
