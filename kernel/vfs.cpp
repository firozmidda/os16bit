
#include<type.h>
#include<fs.h>
#include<vfs.h>
#include<stdio.h>
#include<string.h>
#include<memory.h>
#include<math.h>

PFILESYSTEM root[26];
OPEN_FILE_TABLE OFTAB[5];     

PFILESYSTEM fsptr,tmp_fsptr;

void sleep(int n )
{
     for(int i=0;i<n;i++)
      for(int j=0;i<0xF;j++)
       ;
       
}

int register_fs()
{
    fsptr=NULL;
    tmp_fsptr=NULL;
     
    PFILESYSTEM temp=get_fs_ptr();
    int i=0;
    printf("\n DRIVES ARE ");
    for( i=0;i<26;i++)
     root[i]=0;
    while(temp!=NULL)
    {
          root[temp->dev_name-'A']=temp;
          putc(temp->dev_name);
          putc(':');
          putc(' ');
          temp=temp->next;
                         
    }
    for( i=0;i<5;i++)
    {
        OFTAB[i].flag=ISEMPTY;
    }        
    return true;                 
    
}

int alloc_file_table(OPEN_FILE_TABLE oft)
{

    for(int i=0;i<5;i++)
    {
        if(OFTAB[i].flag == ISEMPTY)
        {
             oft.flag=ALLOC;                   
             OFTAB[i]=oft;
             return i+1;
        }
    }
    return FAILURE;                      
                                    
}


int vfs_mount(char ch)
{   
   
    
    if(ch>='A' && ch<='Z')
    {
        ch='a'+(ch-'A');       
        printf("%c ",ch);      
        printf(" root 0 =%lx=%lx ",root[0],root[ch-'a']);   
    }           
     
    PFILESYSTEM ptr=root[ch-'a'];
    if(!ptr)
      return FAILURE;   
     
    fs_mount(ptr,ch);
    fsptr=ptr;
    printf(" ptr root = %lx ",ptr->root_dir->child);
    
    return true;
           
} 
int open_dir(char *name)
{
     int flag=0; 
     PENTRY temp=fsptr->cur_dir->child;
     if(name[0]==0)
      return 0;
     while(temp!=NULL)
     {
           if(strcmp(name,temp->name)==0)
           {
                if(!(temp->type & DIRECTORY))
                 return flag;                                         
               
                fs_open_dir(fsptr,temp);
                fsptr->cur_dir=temp;
                update_path(fsptr,name,ADD);
                flag=1;
                break;
           }
           temp=temp->next; 
                                        
     }  
     
     return flag;          
    
    
} 
int vfs_close_root()
{
    PENTRY tmp1,tmp2,tmp3;
    tmp1=fsptr->root_dir;
    while(tmp1!=fsptr->cur_dir)
    {
     ;                          
                                
    } 
                               
    return 0;
}    
  
int vfs_close_dir()
{
    PENTRY tmp1,tmp2,tmp3;
    tmp3=fsptr->cur_dir->child;
    
    tmp1=fsptr->cur_dir;
    
    if(tmp1->parent==NULL)
     return FAILURE;
    
    tmp1=fsptr->cur_dir;
    
   
    while(tmp3!=NULL)
    {
         tmp2=tmp3;
         tmp3=tmp3->next;
         free(tmp2);
    } 
    
    fsptr->cur_dir=tmp1->parent;
   
    update_path(fsptr,0,SUB);
    
    return true;
    
}                    
     

int vfs_chdir(char *path)
{
    int err=0;
    char name[15];
    setpath(path,name);
   
    err=open_dir(name);
    
    restore_dir();
  
    return err;
}       

int vfs_view_dir(char *path)
{
    
     PENTRY temp;
     char name[15];
     char attrib[10];
     int i=0;
     setpath(path,name);
   
     open_dir(name);   
     temp=fsptr->cur_dir->child;
     
     printf("\n");
        
     while(temp!=NULL)
     {
                      
            attrib[0]='[';
            attrib[1]=0;
            if(temp->type & READ_ONLY)
             strcat(attrib,"R");
            if(temp->type & HIDDEN)
              strcat(attrib,"H");
            if(temp->type & SYSTEM)
             strcat(attrib,"S");
            if(temp->type & VOLUME)
              strcat(attrib,"V");
            if(temp->type & DIRECTORY)    
              strcat(attrib,"D");
            if(temp->type & ARCHIVE)
              strcat(attrib,"F");
            strcat(attrib,"]"); 
            printf("%17S %7s ",temp->name,attrib); 
                        
            i++;
            if(i==3)
            {
               i=0;       
               printf("\n");
            }
           temp=temp->next;            
         
     }
     restore_dir();
     return true;
}                      

char far *get_path()
{
     if(fsptr==NULL)
      return NULL;
      
     return fsptr->path;
}        

char *setpath(char *path,char *name)
{
     tmp_fsptr=fsptr;
     
     char token[25];
     int i=0;
     if(path[1]==':')
     {
         if(vfs_mount(path[0])==FAILURE)
         {
              fsptr=tmp_fsptr;
              return NULL;
         }
         path+=2;
         if(*path=='\\')
          path++;
     }
     while(*path)
     {
          if(*path=='\\')
          {
              path++;           
              token[i]=0;
              i=0;
              
              open_dir(token);
          }
          token[i]=*path;
          i++;
          path++;
     }
     token[i]=0;
     strcpy(name,token);
     return name;                                              
     
}

int restore_dir()
{
    fsptr=tmp_fsptr;
    return true;
    
}

int vfs_open_file(char *path,int mode)
{
    int flag=0,index=0;
    char name[20];
    setpath(path,name);
    
    u8 far *addr;
    PENTRY temp=fsptr->cur_dir->child;
    OPEN_FILE_TABLE oft;    
    
    while(temp!=NULL)
    {
           if(strcmp(name,temp->name)==0)
           {
                if(!(temp->type & 0x10))
                {
                    if(temp->size>0xFFFF)
                    {
                        printf("\n  Sorry  This File Size Is Too Large For This OS ");
                        flag=0;
                        break;                 
                    }                                     
                    addr=(u8 far *)farmalloc(temp->size+8);
                    if(addr==NULL)
                     printf("\n Cannot Allocate Memory For Opening A File ");
                                       
                    if(fs_open_file(fsptr,temp,addr)!=0)
                    {
                         flag=0;
                         break;
                    }                                    
                    flag=1;
                    strcpy(oft.name,temp->name);
                    oft.buffer=addr;
                    oft.cur_loc=0;
                    oft.mode=mode;
                    oft.size=temp->size;
                    oft.fsptr=fsptr;
                    break;
                }   
           }
           temp=temp->next; 
                                        
     }
     if(!flag)
     {
         printf("\n Cannot Open File ");
     }
     else
     {
         
        index=alloc_file_table(oft);
        restore_dir();
        return index;     
         
     }    
     return 0;         
    
    
} 

int vfs_read(int index,char *buffer,int size)
{
      for(int i=0;i<size;i++)
      {
           putc(OFTAB[index-1].buffer[i]);                
               
      }
      return 0;   
}                


   

int vfs_exec_process(char *path)
{
    
    int flag=0;
    char name[20];
    setpath(path,name);
    
    u32 addr;
    u16 seg,off;
    
    
    PENTRY temp=fsptr->cur_dir->child;
    
    while(temp!=NULL)
    {
           if(strcmp(name,temp->name)==0)
           {
                if(!(temp->type & 0x10))
                {
                    addr=alloc_block(20*1024);            
                    fs_open_file(fsptr,temp,(u8 far *)addr);
                    flag=1;
                    break;
                }   
           }
           temp=temp->next; 
                                        
     }
     if(!flag)
     {
         printf(" Cannot Load Process ");
     }
     else
     {
         seg=FP_SEG(addr);
         off=FP_OFF(addr);
         exec(seg,0x0000); 
        
     }    
     
     
     restore_dir();
    return 0;
    
    
}    

                
 
