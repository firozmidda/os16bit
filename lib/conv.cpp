
#include<type.h>
#include<conv.h>
#include<math.h>

char *digits="0123456789abcdef";



int atoi(char *str)
{
    int i=0,flag=0;
    int res=0;
   
    if(str[i] == '-')
    {
        flag=1;
        i++;
    }    
    
    while(str[i])
    {
          res=res*10+(str[i]-'0');
          i++;
    } 
    if(flag)
     res=-res;           
    return res;
}
/*
int atoi(char far *str)
{
    int i=0,flag=0;
    int res=0;
    if(str[i]=='-')
    {
        flag=1;
        i++;
    }    
    
    while(str[i])
    {
          res=res*10+(str[i]-'0');
          i++;
    }
    if(flag)
     res=-res;           
                
    return res;
}  

long atol(char *str)
{
    int i=0,flag=0;
    long res=0;
    if(str[i]=='-')
    {
        flag=1;
        i++;
    }    
    
    while(str[i])
    {
          res=mul32(res,10)+(str[i]-'0');
          i++;
    }
    if(flag)
     res=-res;           
                
    return res;
}  

long atol(char far *str)
{
    int i=0,flag=0;
    long res=0;
    if(str[i]=='-')
    {
        flag=1;
        i++;
    }    
    
    while(str[i])
    {
          res=mul32(res,10)+(str[i]-'0');
          i++;
    }            
    if(flag)
     res=-res;           
    
    return res;
}  

  
*/
char *itoa(int num,char *str,int base)
{
     int i=8,rem=0;
     int flag=0;
    
     if(num<0)
      flag=1;
    
     str[i--]=0;  
     do  
     {        
         str[i--]=digits[num%base];
         num=num/base;
     }
     while(num!=0  );
     if(flag)    
      str[i--]='-';   
    return &str[i+1];
}
/*
char *uitoa(u16 num,char *str,int base)
{
     int i=8,rem=0;
    
     str[i--]=0;
     do  
     {        
         str[i--]=digits[num%base];
         num=num/base;
     }
     while(num!=0);       
    return &str[i+1];
}          
          
*/
char *ltoa(long num,char *str,int base)
{
     int i=15,rem=0;
     int flag=0;
   
     if(num<0)
      flag=1;
   
     str[i--]=0;
     
     if(num<0)
      str[i--]='-';
      
     do  
     {        
         str[i--]=digits[mod32(num,base)];
         num=div32(num,base);
     }
     while(num!=0); 
     if(flag)
      str[i--]='-';      
    return &str[i+1];
}          
/*
char *ultoa(u32 num,char *str,int base)
{
     
     int i=15,rem=0;
    
     str[i--]=0;
     do  
     {        
         str[i--]=digits[mod32(num,base)];
         num=div32(num,base);
     }
     while(num!=0);    
        
    return &str[i+1];
}  
*/
