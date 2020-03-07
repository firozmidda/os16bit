

#include<type.h>
#include<string.h>

/************ START OF  S T R C M P  *********************************/

int strcmp (const char* str1, const char* str2) {

	
	while (*str1 == *str2++) {
		if (*str1++ == '\0') {
			return 0;
		}
	}
	if (*str1 == '\0') return -1;
	if (*--str2 == '\0') return 1;
	return (unsigned char) *str1 - (unsigned char) *str2;

}

int strcmp (const char far * str1, const char far * str2) {

	
	while (*str1 == *str2++) {
		if (*str1++ == '\0') {
			return 0;
		}
	}
	if (*str1 == '\0') return -1;
	if (*--str2 == '\0') return 1;
	return (unsigned char) *str1 - (unsigned char) *str2;

}
int strcmp (const char* str1, const char far* str2)
{

	
	while (*str1 == *str2++) {
		if (*str1++ == '\0') {
			return 0;
		}
	}
	if (*str1 == '\0') return -1;
	if (*--str2 == '\0') return 1;
	return (unsigned char) *str1 - (unsigned char) *str2;
}

int strcmp (const char far* str1, const char* str2) {

	
	while (*str1 == *str2++) {
		if (*str1++ == '\0') {
			return 0;
		}
	}
	if (*str1 == '\0') return -1;
	if (*--str2 == '\0') return 1;
	return (unsigned char) *str1 - (unsigned char) *str2;

}

/*****************END OF S T R C M P **************/


/****************START OF S T R N C M P **************/
/*
int strncmp(const char *str1,const char *str2,int size)
{
   	int res=0,i=0;
	while(str2[i]&&(i<size))
	{
        if(str1[i]!=str2[i])
        {
            res=*(u8 *)str1-*(u8 *)str2;
            break;
         }
         i++;
     }
    if (res < 0)
		res = -1;
	if (res > 0)
		res = 1;
	return res;	
 
}   

int strncmp(const char far *str1,const char far *str2,int size)
{
    int res=0,i=0;
	while(str2[i]&&(i<size))
	{
        if(str1[i]!=str2[i])
        {
            res=*(u8 far *)str1-*(u8 far *)str2;
            break;
         }
         i++;
     }
    
    if (res < 0)
		res = -1;
	if (res > 0)
		res = 1;
	return res;	
 
}

int strncmp(const char far *str1,const char *str2,int size)
{
    int res=0,i=0;
	while(str2[i]&&(i<size))
	{
        if(str1[i]!=str2[i])
        {
            res=*(u8 far *)str1-*(u8 *)str2;
            break;
         }
         i++;
     }
    
    if (res < 0)
		res = -1;
	if (res > 0)
		res = 1;
	return res;	
 
} 

int strncmp(const char *str1,const char far *str2,int size)
{
    int res=0,i=0;
	while(str2[i]&&(i<size))
	{
        if(str1[i]!=str2[i])
        {
            res=*(u8 *)str1-*(u8 far *)str2;
            break;
         }
         i++;
     }
    
    if (res < 0)
		res = -1;
	if (res > 0)
		res = 1;
	return res;	
 
}   
*/
/***************END OF  "S T R N C M P" ****************/

/*************START OF S T R C P Y ********************/
char *strcpy(char *s1, const char *s2)
{
    char *s1_p = s1;
    while (*s1++ = *s2++);
    return s1_p;
}
char far *strcpy(char far *s1, const char far *s2)
{
    char far *s1_p = s1;
    while (*s1++ = *s2++);
    return s1_p;
}

char far *strcpy(char far *s1, const char *s2)
{
    char far *s1_p = s1;
    while (*s1++ = *s2++);
    return s1_p;
}
char *strcpy(char *s1, const char far *s2)
{
    char *s1_p = s1;
    while (*s1++ = *s2++);
    return s1_p;
}

/*---------------------END OF S T R C P Y ----------*/ 

/******************START OF S T R N C P Y **************/
/*
char *strncpy(char *s1,const char *s2,int size)
{
     char *s1_p=s1;
     while((*s1++ = *s2++)&& size)
      size--;
     return s1_p;
           
}
char far *strncpy(char far *s1,const char far *s2,int size)
{
     char far *s1_p=s1;
     while((*s1++ = *s2++)&& size)
      size--;
     return s1_p;
           
}

char far *strncpy(char far *s1,const char *s2,int size)
{
     char far *s1_p=s1;
     while((*s1++ = *s2++)&& size)
      size--;
     return s1_p;
           
}
char *strncpy(char *s1,const char far *s2,int size)
{
     char *s1_p=s1;
     while((*s1++ = *s2++)&& size)
      size--;
     return s1_p;
           
}*/

/*********** END OF S T R N C P Y  *********/

/**********START OF S T R C A T  *********/

char *strcat(char *s1,const char *s2)
{
     char *s1_p=s1;
   
     while(*s1)
      s1++;
      
    for(;*s2;s2++,s1++)
     *s1=*s2;
   
     *s1=0; 
     return s1_p;
}    
/* 
char far *strcat(char far *s1,const char far *s2)
{
     char far *s1_p=s1;
   
     while(*s1)
      s1++;
      
    for(;*s2;s2++,s1++)
     *s1=*s2;
   
     *s1=0; 
     return s1_p;
}      

char far *strcat(char far *s1,const char *s2)
{
     char far *s1_p=s1;
   
     while(*s1)
      s1++;
      
    for(;*s2;s2++,s1++)
     *s1=*s2;
   
     *s1=0; 
     return s1_p;
}   
  
char *strcat(char *s1,const char far *s2)
{
     char *s1_p=s1;
   
     while(*s1)
      s1++;
      
    for(;*s2;s2++,s1++)
     *s1=*s2;
   
     *s1=0; 
     return s1_p;
}     
*/
/**************** END OF S T R C A T  *********/

/****************START OF  S T R L E N  ************/

int strlen(char *str)
{
    int size=0;
    while(str[size++]);
    
    return size;
}    
/*
int strlen(char far *str)
{
    int size=0;
    while(str[size++]);
    
    return size;
} */   

/*************END OF  S T R L E N **************/


/*__________________________________________________________________
    
                M E M O R Y   F U N C T  I O N 
  __________________________________________________________________*/
 
 
 
 /****************START OF  M E M C P Y ****************************/
 /*                
void *memcpy(void *dest, const void *src,int count)
{
    const char  *sp = (const char *)src;
    char *dp = (char *)dest;
    for(; count != 0; count--)
    {         
         *dp++ = *sp++;
    }       
    return dest;
}
*/
void far *memcpy(void far *dest, const void far *src,int count)
{
    const char far  *sp = (const char far *)src;
    char far *dp = (char far *)dest;
    for(; count != 0; count--)
    {         
         *dp++ = *sp++;
    }       
    return dest;
}
void far *memcpy(void far *dest, const void  *src,int count)
{
    const char   *sp = (const char  *)src;
    char far *dp = (char far *)dest;
    for(; count != 0; count--)
    {         
         *dp++ = *sp++;
    }       
    return dest;
}
void  *memcpy(void  *dest, const void far *src,int count)
{
    const char far *sp = (const char far *)src;
    char  *dp = (char  *)dest;
    for(; count != 0; count--)
    {         
         *dp++ = *sp++;
    }       
    return dest;
}
     
/************* END OF M E M C P Y ***********/

/***************START OF M E M S E T  ***********/

void *memset(void *dest,char data,int count)
{
     char *dp=(char *)dest;
     for(;count!=0;count--)
      *dp++=data;
     
     return dest;
}     

void far *memset(void far *dest,char data,int count)
{
     char far *dp=(char far *)dest;
     for(;count!=0;count--)
      *dp++=data;
     
     return dest;
}    

/************** END OF  M E M S E T  ***************/

     
