
#include<type.h>

#ifndef _FAT32_H_
 #define _FAT32_H_
 #include<fs.h>
 
 
 PENTRY make_fat32_dtree(PFILESYSTEM ptr,PENTRY entry);
 
 int open_fat32_file(PFILESYSTEM ptr,PENTRY entry,u8 far *buffer);
 
 
 
 #endif
 
 
