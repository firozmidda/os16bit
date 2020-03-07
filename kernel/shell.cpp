#include<type.h>
#include<ctype.h>
#include<conv.h>
#include<string.h>
#include<vdu.h>
#include<stdio.h>
#include<keyboard.h>
#include<vfs.h>
#include<memory.h>


char line[100];
int pos=0;


enum { VAR,NUM,DELIM};

struct SYMBOL
{
     int type;
     char name[40];
     int value;
       
}sym;

SYMBOL SYMTAB[10];
int sym_size=0;

void eval_exp1(int &result);
 
      
int isdelim(char ch)
{
    char *str="\n\r -=+-*/=.,;\0";
    if(ch==0)
     return 1;
    while(*str)
    {
       if(*str==ch)
        return 1;
      str++;
   }
   return 0;
}      
                
    
int get_path(char *str)
{
    while(line[pos]==' ' && line[pos]!='\0')
     pos++;
     
    for(int  i=0;i<80 && line[pos]!=0;i++)
    {
           str[i]=line[pos];
           pos++;
    }
    str[i]=0;
    return 0;      
    
}
void getsym()
{
    
    int i=0;
    while(line[pos]==' ' && line[pos]!='\0')
     pos++;
     
   
    if(isalpha(line[pos]))
    {
                   
         while(!isdelim(line[pos]))
         {
              sym.name[i]=line[pos];
              i++;
              pos++;
              
         }
         sym.name[i]=0;
         sym.type=VAR;
         sym.value=0;
                                               
    }
    else if(isdigit(line[pos]))
    {
         while(isdigit(line[pos]))
         {
              sym.name[i]=line[pos];
              i++;
              pos++;
              
         }
         sym.name[i]=0;
         sym.type=NUM;
         sym.value=atoi(sym.name);
    }
    else
    {
        sym.name[0]=line[pos];
        sym.name[1]=0;
        sym.type=DELIM;
        sym.value=0;                           
        pos++;
    }   
     
     
}    



void add(SYMBOL &s)
{
     int flag=0;
     for(int i=0;i<sym_size;i++)
     {
        if(strcmp(SYMTAB[i].name,s.name)==0)
        {
            flag=1;
            SYMTAB[i].value=s.value;
                
        }
     }
     if(!flag)
     {
        SYMTAB[sym_size]=s;
        sym_size++;
     }
} 
int find(char *str)
{
     for(int i=0;i<sym_size;i++)
     {
         if(strcmp(SYMTAB[i].name,str)==0)
          return i;
      }
      return -1;
}              
                                                               


void atom(int &result)
{
    int found;
    switch(sym.type)
    {
        case VAR:
             found=find(sym.name);
             if(found!=-1)
             {
                result=SYMTAB[found].value;
                printf("res= %d ",result);
             }  
             else
              printf("Undefine Symbol:%s ",sym.name);
             getsym();
              break;                         
                   
        case NUM:
             result=sym.value;
             printf(" sym value =%d ",result );
             
             getsym();  break;
        default:
               printf("error");
    } 
}                     
                         
void eval_exp4(int &result)
{
     if(sym.name[0]=='(')
     {
         getsym();
         eval_exp1(result);
         if(sym.name[0]!=')')
         {
           printf("ERROR parenthesis require ");
           getsym();
         }  
          
     }
     else
      atom(result);
}     

void eval_exp3(int &result)
{
     char op;
     int temp,res=1;
     eval_exp4(result);
     if(sym.name[0]=='^')
     {
         getsym();
         eval_exp4(temp);
         for(int i=0;i<temp;i++)
         {
            res=res*result;
         }
         result=res;
           
     }
}                        
                       

void eval_exp2(int &result)
{
     char op;
     int temp;
     eval_exp3(result);
     while((op=sym.name[0])=='*'||op=='/'||op=='%')
     {
         getsym();
         eval_exp3(temp);
         switch(op)
         {
             case '*':
                  result=result*temp; break;
             case '/':
                  result=result/temp; break;
            case '%':
                  result=result%temp; break;
               
         }
     }
}                        


void eval_exp1(int &result)
{
     char op;
     int temp;
     eval_exp2(result);
     while((op=sym.name[0])=='+'||op=='-')
     {                                  
         getsym();
         eval_exp2(temp);
         switch(op)
         {
             case '+':
                  result=result+temp; break;
             case '-':
                  result=result-temp; break;
         }
     }
}                        
                        
                                          
   


int eval_exp()
{
    int result;
    eval_exp1(result);
    return result;
}    
         


int cd()
{
     char path[50];
     get_path(path);
     printf(" path =%s ",path); 
     if(path[0]=='.')
     {
        if(strcmp(path,"..")==0)
         vfs_close_dir();
     }  
     
     else if(vfs_chdir(path)==0)
     {
        printf(" cannot Open Directory ");
        return 0;
     }
     return 0;
}     

int help()
{
    
   printf("\n The Commands Are :-");
   printf(" CD DIR EXIT TYPE ");
   return 0;
    
}           
        
        
int dir()
{
    char path[50];
    get_path(path);
    
    if(vfs_view_dir(path)==0)
    {
        printf(" cannot View dir ");
        return 0;
    }
    return 0;
}

int view_file()
{
    char path[50];
    get_path(path);
    
    if(vfs_open_file(path,1)==0)
    {
        printf(" cannot View file ");
        return 0;
    }
    return 0;
    
    
}    
int print()
{
    char path[50];
    get_path(path);
    char var[20];
    int j=0,found;
    int result=0;
    
    int i=0;
    for(i=0;(path[i]==' ')&&(i<50);i++);
    
    if(path[i]=='\"')
    {
        
        i++;             
        for(i;path[i]!=0;i++)
        {
            if(path[i]=='\"')
             break;
            if(path[i]=='$')
            {
               j=0;
               printf(" comes in variable ");             
               while(path[i]!=' ' && i<50)
               {
                   var[j++]=path[i++];
               }
               var[j]=0;
               found=find(var);
               printf(" found =%d ",found);
               if(found!=-1)
               {        
                  printf(" comes  inside ");          
                  result=SYMTAB[found].value;
                   printf("res= %d ",result);
               }  
               else
                printf("Undefine Symbol:%s ",var);
               
                                
                                             
            }                 
            putc(path[i]);
        }
    
    }
    
    //printf(" Error in Printf Satatement ");
    return 0;                                  
    
}    


int init_shell()
{
      
       char buffer[60];
       char far *str; 
       while(1)
       {
           pos=0;
           str=get_path();
           if(str==NULL)
            buffer[0]=0;
           else 
           strcpy(buffer,str);
           printf("\n%s>",buffer);
           gets(line);
          
           getsym();
           if(sym.name[1]==':')
           {
              if(vfs_mount(sym.name[0])==0)
               printf(" Cannot mount drive ");
           }                    
           if(strcmp(sym.name,"dir")==0)
            dir();
           else if(strcmp(sym.name,"cd")==0)
            cd();
           else if(strcmp(sym.name,"cls")==0)
            clear_screen();
           else if(strcmp(sym.name,"type")==0)
            view_file();
           else if(strcmp(sym.name,"exit")==0)
            break;
           else if(strcmp(sym.name,"help")==0)
            help(); 
           else if(strcmp(sym.name,"echo")==0)
            print(); 
			else if(strcmp(sym.name,"mem")==0)
			 view_table();
           else if(sym.name[0]!=0)
           {
                SYMBOL tsym,tsym1;
                if(sym.type==NUM)
                {
                                 
                     tsym1.value=eval_exp();
                     printf(" %d ",tsym1.value);
                     
                                        
                }
                else if(sym.type==VAR)
                {
                 
                   strcpy(tsym.name,sym.name);
                   getsym();
                   if(sym.name[0]=='=')
                    {
                        printf(" Comes In shell");               
                        getsym();
                       tsym1.value=eval_exp();
                       add(tsym1);               
                    }
                    else
                    {
                       printf(" Commond Not Found %s ",tsym.name);
                    }
                }         
               
           }     
           
            
       }   
       return 0;
}      
