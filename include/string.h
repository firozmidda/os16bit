
#include<type.h>


#ifndef _STRING_H_
 #define _STRING_H_
 
 
/* 

  Due To Tiny Memory Model We Have To Write all The Function 2 or 4 times 
    
*/
/* String  functions */
 
int strcmp (const char* str1, const char* str2);        // both local
int strcmp (const char far *str1,const char far *str2); // far far 
int strcmp (const char* str1, const char far *str2);    // local far 
int strcmp (const char far *str1,const char *str2);     // far local



int strncmp (const char *str1,const char *str2,int size);
int strncmp (const char far *str1,const char far *str2,int size);
int strncmp (const char far *str1,const char *str2,int size);
int strncmp (const char *str1,const char far *str2,int size);



char *strcpy (char *s1, const char *s2);
char far *strcpy (char far *s1,const char far *s2);
char far *strcpy (char far *s1, const char *s2);
char *strcpy (char *s1, const char far *s2);



char *strncpy (char *s1,const char *s2,int size);
char far *strncpy (char far *s1,const char far *s2,int size);
char far *strncpy (char far *s1,const char *s2,int size);
char *strncpy (char *s1,const char far *s2,int size);


char *strcat(char *s1,const char *s2);
char far *strcat(char far *s1,const char far *s2);
char far *strcat(char far *s1,const char *s2);
char *strcat(char *s1,const char far *s2);

int strchr (char *str, int character );
int strchr (char far *str, int character );


int strlen(char *str);
int strlen(char far *str);


/* Memory Function */

void *memcpy(void *dest, const void *src,int count);
void far *memcpy(void far *dest, const void far *src,int count);
void far *memcpy(void far *dest, const void  *src,int count);
void *memcpy(void  *dest, const void far *src,int count);


void *memset(void *dest,char data,int count);
void far *memset(void far *dest,char data,int count);

 
 
#endif _STRING_H_ 
