

#include<type.h>
#ifndef _VDU_H_
 #define _VDU_H_
 
 
 
int init_vdu();
int set_6845(u16 reg,int data);
int write_vdu(u16 row,u16 col,char ch,u16 color);
void set_cursor(int row,int colum);
void scroll_up();
int clear_screen();
 /*
 int init_vdu();

 int write_vdu(int row,int col,char *str);
 int put_buff(char ch,int color);
 int clear_vdu();
 int flush_buff();
 int scroll_vdu();
 int move_cursor();
 int clear_screen();
 */
 
#endif


 
 
