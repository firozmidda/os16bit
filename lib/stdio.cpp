#include<type.h>
#include<conv.h>
#include<vdu.h>
#include<stdarg.h>
#include<ctype.h>
#include<string.h>

u16 row=0,col=0,color=0x0f;

enum{NEGATIVE=1,DATA=2,LONG=4};


int start_vdu()
{
    init_vdu();
    return 0;
}        

int putc(char ch)
{
    
    if(row>=25)
    {
       scroll_up();        
       row=24;
    }    
    switch(ch)
    {
              
         case '\n': row++; col=0; break;
         case '\t': col+=4; break;
         case '\b': col--; ch=' ';
              write_vdu(row,col,ch,color);
               break;
         
         default :
                write_vdu(row,col,ch,color);   
                 if(col==80)
                 {
                    row++;
                    col=0;
                 }   
                 else
                  col++;
                 
                 break;
    }
    set_cursor(row,col);
    return (u16)ch;
}                     

int puts(char *str)
{
    while(*str)
    {
       putc(*str);
       str++; 
    }
    return 0;
}               

int gotoxy(u16 row,u16 col)
{
    set_cursor(row,col);
    return 0;
}    

int putint(int num)
{
    char tmp[17];
    puts(itoa(num,tmp,10));
    return 0;
}  
int puthex(int num)
{
    char tmp[17];
    puts(itoa(num,tmp,16));
    return 0;
}    


int print_width(char *str,char *flag,int *data)
{
    
   
    int len=strlen(str);
    if(*flag&DATA)
    {
       if(!((*flag)&NEGATIVE))
       {
           for(int i=0;i<(*data-len);i++)                  
            putc(' ');
            puts(str);
       }
       else
       {
          puts(str);
          for(int i=0;i<(*data-len);i++)
           putc(' ');
       }
     }
     else
     {
         if(*str==0)
          puts("null");
          puts(str);
     }       
    *flag=0;
    *data=0;
    return 0;
}             
                                           
    
 


  
int printf(char *fmt,...)
{
    char *ptr,str[16],strnum[20];
    
    int i,data=0,base=0;
    u16 u,j=0;
    char *s,flag=0;
    char far *f_s;
    
    long l;
    u32 ul;
    char tmp[20];
    
    
    va_list arg;
    va_start(arg,fmt);
    ptr=fmt;
    for(ptr=fmt;*ptr;ptr++)
    {
        if(*ptr!='%')
        {
           putc(*ptr); continue;
        }
        // *ptr is "%" so increment ptr
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
                    case 'c':  str[0]=va_arg(arg,char); str[1]=0; break;
             
             
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
                      strcpy(str,ltoa(l,tmp,base));
                  }
                  else 
                  {
                       i=va_arg(arg,int);
                       if(i<0)
                       {
                          str[j++]=0; i=-i;
                       }
                       str[j]=0;
                       strcpy(str,itoa(i,tmp,base));
                  }
                  break;
             case 'S':
                     f_s=va_arg(arg,char far *);
                     strcpy(str,f_s);
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
                      strcpy(str,ltoa(ul,tmp,base));
                  }
                  else 
                  {
                       u=va_arg(arg,u16);
                       strcpy(str,itoa(u,tmp,base));
                  }
                  break;
                
          }//switch close                      
          print_width(str,&flag,&data);
      }//   !for close                                                 
    
    return 0;
}
