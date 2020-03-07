
#include<type.h>
#include<vdu.h>
#include<stdio.h>
#include<stdarg.h>
#include<ctype.h>
#include<conv.h>
#include<string.h>



int color=0x0f;
enum{NEGATIVE=1,DATA=2,LONG=4,UNSIGNED=8};

int puts(char *str)
{
    
    int i=0;
    while(str[i])
    {
        put_buff(str[i++],color);
    }
    flush_buff();
    return i;                                              
    
}

int puts(char far *str)
{
    
       int i=0;
    while(str[i])
    {
        put_buff(str[i++],color);
    }
    flush_buff();
    return i; 
}   

int putc(char ch)
{
    put_buff(ch,color);
    flush_buff();
     return SUCCESS;         
     
} 
/* 
int printf(char * frmt,...)
{

   char *p,c;
   int i,j=0;
   unsigned u;
   char *s;
   char buff[20];
   u32 ul;
   va_list argp;
   
   va_start(argp, frmt);

  p=frmt;
 for(p=frmt; *p!='\0';p++)
{
  if(*p!='%')
{
             
    putc(*p);continue;
}

p++;

 switch(*p)
 {
  case 'c' : i=va_arg(argp,u8);putc(c); break;
  case 'd' : i=va_arg(argp,int); puts(itoa(i,buff,10)); break;
  case 'o': u=va_arg(argp,unsigned int); puts(itoa(u,buff,8));break;
  case 's': s=va_arg(argp,char *); puts(s); break;
  case 'u': u=va_arg(argp, unsigned int); puts(uitoa(u,buff,10));break;
  case 'x': u=va_arg(argp, unsigned int); puts(uitoa(u,buff,16));break;
  case 'p': ul=va_arg(argp,u32); puts(ultoa(ul,buff,16)); break;
  case 'l': ul=va_arg(argp,u32); puts(ultoa(ul,buff,10)); break;
  case '%': putc('%');break;
 }
}

 va_end(argp);
 return true;
}

/*

int printf(char * frmt,...)
{

   char *p;
   int i;
   unsigned u;
   char *s;
   char buff[20];
   u32 ul;
   va_list argp;
   
   va_start(argp, frmt);

  p=frmt;
 for(p=frmt; *p!='\0';p++)
{
  if(*p!='%')
{
 putc(*p);continue;
}

p++;

 switch(*p)
 {
  case 'c' : i=va_arg(argp,int); putc(i); break;
  case 'd' : i=va_arg(argp,int); puts(itoa(i,buff,10)); break;
  case 'o': u=va_arg(argp,unsigned int); puts(itoa(u,buff,8));break;
  case 's': s=va_arg(argp,char *); puts(s); break;
  case 'u': u=va_arg(argp, unsigned int); puts(uitoa(u,buff,10));break;
  case 'x': u=va_arg(argp, unsigned int); puts(uitoa(u,buff,16));break;
  case 'p': ul=va_arg(argp,u32); puts(ultoa(ul,buff,16)); break;
  case 'l': ul=va_arg(argp,u32); puts(ultoa(ul,buff,10)); break;
  case '%': putc('%');break;
 }
}

 va_end(argp);
 return true;
}


*/

int print_width(int *flag,int *data,char *str)
{
    int len=0;
    
    while(str[len])
     len++;
    
    if((*flag) & NEGATIVE )
    {
        for(int i=0;i<(*data-len);i++)
          put_buff(' ',color);
    }
    
    puts(str);
     
    if(!((*flag) & NEGATIVE ))
    {
         for(int i=0;i<(*data-len);i++)
          put_buff(' ',color);
    }
    *flag=0;
    *data=0;                       
    return 0; 
    
    
}    


int printf(char *fmt,...)
{
    
    char ch;
    
    char *ptr,*s,strnum[16],tmp[20],str[50];
    char far *f_s;
    int i,flag=0,data=0,base=0;
    u16 ui;
    long l;
    u32 ul;
    
    va_list arg;
    va_start(arg,fmt);
    
    for(ptr=fmt;*ptr;ptr++)
    {
        
        if(*ptr!='%')
        {
            put_buff(*ptr,color);
            continue;
        }
        ptr++;   // move to next char it is %
       
        if(*ptr=='-')
        {
            flag=flag|NEGATIVE;
            ptr++;
        }
           
        if(isdigit(*ptr))
        {
              int j=0;
              while(isdigit(*ptr))
              {
                  strnum[j]=*ptr;
                  j++,ptr++;
              
               }
               putc(*ptr);
               strnum[j]=0;
               data=atoi(strnum);
               flag=flag|DATA;
               
            
        }
        
        if(*ptr=='l')
        {
              flag=flag|LONG;
              ptr++;
        }
        
        
        switch(*ptr)
        {
            
            case 'c': ch=va_arg(arg,u8); 
                       str[0]=ch; str[1]=0; break;
            
            case 'd':
            case 'u': 
            case 'x': 
                   if(*ptr == 'd') base=10;
                   else if(*ptr == 'u') { base=10; flag|=UNSIGNED;}
                   else if(*ptr == 'x') { base=16;  flag|=UNSIGNED; }
                   else base=10;
                   if(flag & LONG)
                   {
                        if(flag & UNSIGNED )
                        {
                             ul=va_arg(arg,u32);
                             strcpy(str,ultoa(ul,tmp,base));
                        }
                        else
                        {
                            l=va_arg(arg,long);
                            strcpy(str,ltoa(l,tmp,base));
                        }
                   }                    
                   else 
                   {
                        
                        if(flag & UNSIGNED )
                        {
                             ui=va_arg(arg,u16);
                              strcpy(str,uitoa(ui,tmp,base));
                        }
                            
                        else
                        {
                            i=va_arg(arg,int);
                           
                            strcpy(str, itoa(i,tmp,base));
                        }
                   }
                   break;
           
                 
            case 'S':
                  
                  f_s=va_arg(arg,char far *);
                      strcpy(str,f_s);
                     break;        
            case 's': s=va_arg(arg,char *);
                      strcpy(str,s); 
                   break;
            case '%': 
                     put_buff('%',color);
                      break;       
                   
                   
        }
        
        print_width(&flag,&data,str);
                     
    }
    flush_buff();
    va_end(arg);
    return 0;
}             
                                              
                 
                                                                 
    
    
    


/*

int printf(char *fmt,...)
{
    char *ptr,strnum[10];
    int i=0,flag=0;
    
    
    va_list arg;
    va_start(arg,fmt);
    
    for(ptr=fmt;*ptr;ptr++) 
    {
         if(*ptr!='%')
         {
              switch(*ptr)
              {
                  case '\n':
                  case '\t':
                  case '\b':
                    ctrl_char(*ptr);
                  break;
           
                   default:
                   buffer[i++]=(*ptr)|(color<<8);
               }   
          }
          //ptr is % so increment it
          ptr++;
           
          if(*ptr=='-')
          {
              flag=flag|NEGATIVE;
              ptr++;
          }
           
           if(isdigit(*ptr))
           {
                 int j=0;
           
                while(isdigit(*ptr))
                {
                     strnum[j]=*ptr;
                     j++,ptr++;
              
                 }
                 strnum[j]=0;
                 data=atoi(strnum);
                 flag=flag|DATA;
            
           }
          if(*ptr=='l')
           {
                flag=flag|LONG;
                ptr++;
           } 
           switch(*ptr)
           {
               case 'c':  str[0]=*ptr; str[1]=0; break;
            
               case 'b':     
               case 'd':
                  
                 if(*ptr=='d') base=10;
                 else if(*ptr=='b') base=2;                          
                 j=0; 
                   if(flag&LONG) 
                  {
                      l=va_arg(arg,long);
                      if(l<0)
                      {
                         str[j++]='-'; l=-l;
                      }
                      str[j]=0;
                      strcat(str,ltoa(l,base));
                  }
                  else 
                  {
                       i=va_arg(arg,int);
                       if(i<0)
                       {
                          str[j++]=0; i=-i;
                       }
                       str[j]=0;
                       strcat(str,itoa(i,base));
                  }
                  break;
                  
             case 's': s=va_arg(arg,char *);
                   strcpy(str,s); 
                   break;
             
             case 'x':      
             case 'u': 
                  if(*ptr=='x') base=16;
                  else base=10;
                  str[0]=0;
                   if(flag&LONG) 
                   {
                      ul=va_arg(arg,u32);
                      strcat(str,ltoa(ul,base));
                  }
                  else 
                  {
                       u=va_arg(arg,u16);
                       strcat(str,itoa(u,base));
                  }
                  break;
                
          }//switch close                      
                               
           
      }   
                                                     
    
}
 */   
