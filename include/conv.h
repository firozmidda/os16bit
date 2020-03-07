

#ifndef _CONV_H_
 #define _CONV_H_
 
 int atoi(char *str);
 int atoi(char far *str);
 
 long atol(char *str);
 long atol(char far *str);
 
 char *itoa(int num,char *str,int base);
 char *ltoa(long num,char *str,int base);
 
 u32 strtoul(char *str);
 u32 strtoul(char far *str);
 
 char *ultoa(u32 num,char *str,int base); 
 char *uitoa(u16 num,char *str,int base);
 
 
#endif 
