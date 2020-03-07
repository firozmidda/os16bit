#include<type.h>
#include<ctype.h>

int alnum(int c)
{
     if((c-'0')<9||(c-'A')<26||(c-'a')<26)
       return TRUE;
     return FALSE;
     
      
} 
int isalpha(int c)
{
    if((c>='A' || c>='a' )&&(c<='Z' || c<='z'))
     return TRUE;
    return FALSE;
}

int isdigit(int c)
{
    if(c>='0' && c<='9')
     return TRUE;
    return FALSE;
}

int islower(int c)
{
    if(c>='a'||c<='z')
     return TRUE;
    return FALSE;
} 

int isupper(int c)
{
    if(c>='A'||c<='Z')
     return TRUE;
    return FALSE;
}

int tolower(int c)
{
    if(isupper(c))
     return c+'a';
    return c; 
}

int toupper(int c)       
{
    if(islower(c))
     return c+'A';
    return c;
}
int isarithopr(int c)
{
    switch(c)
    {
       case '+':
       case '-':
       case '*':
       case '/':
       return 1;
       
       default :
               return 0;
    }
}                                                
                   
     
        
