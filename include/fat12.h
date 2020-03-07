
#include<type.h>

#ifndef _FAT12_H_
 #define _FAT12_H_
 #include<fs.h>
 
 
 PENTRY make_fat12_dtree(PFILESYSTEM ptr,PENTRY entry);
int open_fat12_file(PFILESYSTEM ptr,PENTRY entry,u8 far *buffer); 
 
 
 
 
 #endif
 
 
