
#include<type.h>
#include<fs.h>
 #ifndef _VFS_H_
 #define _VFS_H_
 
 
 enum { ISEMPTY=0, ALLOC=1};
 enum {READ_ONLY=0x01,HIDDEN=0x02,SYSTEM=0x04,VOLUME=0x08,DIRECTORY=0x10,ARCHIVE=0x20,DEVICE=0x40};

 
 struct OPEN_FILE_TABLE
 {
      int flag;  
      char name[20];
      int mode;
      u32 size;
      u32 cur_loc;
      u32 cur_pos;
      PFILESYSTEM fsptr;
      u8 far *buffer;
 };
 
  
 
 
 
 char far *get_path();
 char *setpath(char *path,char *name);
 int restore_dir();
 
 int register_fs();
  
 int vfs_mount(char ch);
 int vfs_view_dir(char *path);
 int vfs_chdir(char *path);
 int vfs_close_dir();
 int vfs_open_file(char *path,int mode);
 int vfs_exec_process(char *path);
 int vfs_read(int index,char *buffer,int size);
 
 extern "C" void exec(u16 seg,u16 off);
 
 
 #endif 
