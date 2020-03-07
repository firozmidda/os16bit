
#include<type.h>
#include<ntfs.h>
#include<fs.h>
#include<stdio.h>
#include<math.h>
#include<memory.h>
#include<string.h>


char *file_sig="FILE";
u8 far *filebuff;
u8 far *file_addr;
PENTRY pfront=NULL,prear=NULL,parent=NULL;
PFILESYSTEM fs_ptr;
u32 file_size,cur_size;


int open_file_record(PFILESYSTEM ptr,u32 mft)
{
       
       u32 start_lba,start_sec,sector;
       int clus_sz,error;
              
       start_lba=ptr->start_lba;
       clus_sz=ptr->clus_sz;
       start_sec=mul32(ptr->dntfs.start_mft,clus_sz);
       sector=start_lba+start_sec+mul32(mft,2);
       filebuff=(u8 far *)farmalloc(1024);
       
       //printf(" lba= %p mft=%p ",start_lba,mft);
       error=read_disk(ptr->dptr,sector,2,filebuff);
       
       if(error!=0)
       {
             printf(" Cannot Read Disk %x ",error);
             return 0;
       }
       for(int i=0;i<4;i++)
       {
           if(filebuff[i]!=file_sig[i])
            return FAILURE;
       }
       return SUCCESS;
                
}

int parse_index_entry(u8 far *buff,u16 offset,u32 size)
{
    
    
    INDEX_ENTRY indx_entry;
    ATTR_FILE_NAME far *file_name;
    PENTRY temp;
    int offs=offset;
    int i=0;
    
    while(size>0)
    {
         memcpy(&indx_entry,buff+offs,sizeof(INDEX_ENTRY));
         
         if(indx_entry.Size==0)
          break;
         size-=indx_entry.Size;
         
         file_name=(ATTR_FILE_NAME far *)(buff+offs+0x10);
         
        // printf(" Flag =%x ",indx_entry.Flags);
           
         
            
         if((file_name->NameSpace & 0x02)&&(file_name->NameLength)&&(indx_entry.FileReference[0]!=0))
         {
                                                                
               temp=(PENTRY)farmalloc(sizeof(ENTRY));
              // printf(" temp =%p ",temp);
               temp->next=NULL;
               temp->parent=parent;  
               temp->child=NULL;
                      
               
              // printf(" len= %x ",file_name->NameLength);
               for(i=0;i<file_name->NameLength && i<15 ;i++)
               {
                   temp->name[i]=file_name->Name[i];
                   
               } 
                 
               temp->name[i]=0;
               if(pfront)
               {
                   if(strcmp(prear->name,temp->name)==0)
                   {
                         free(temp);
                         offs+=indx_entry.Size;
                         continue; 
                   }                                         
               }          
                       
              // puts(temp->name);
               temp->type=0;
               if(file_name->Flags&0x10000000)
                temp->type|=0x10;
               
                temp->type|=file_name->Flags;
               
               temp->size=file_name->RealSize[0];
               temp->mft=indx_entry.FileReference[0];
              // printf(" %p ",temp->mft);
             //  printf(" [");
               if(pfront==NULL)
               {
                   pfront=temp;
                   prear=temp;
                 //  printf(" pfront =%p ",pfront);
               }
               else
               {
                   prear->next=temp;
                   prear=temp;
               }
               //printf("]");
         }                                
         offs+=indx_entry.Size;       
                     
                 
    }            
    return 0;
    
}    




int parse_indexroot(int offset,int len)
{
    INDEX_ROOT indx_root;
        
    int offs=offset;
    int indxoffs;
        
    memcpy(&indx_root,filebuff+offs,sizeof(INDEX_ROOT));
    indxoffs=offs+indx_root.entry_offs+0x10;  //beacuse index header  is self present int index root 
    
    parse_index_entry(filebuff,indxoffs,indx_root.szofindx_entry);
    
     return true;
    
    
}   

int parse_indexheader(u8 far *buff)
{
    INDEX_HEADER indx_hdr;
    char *str="INDX";
    
    
    int offset;
    memcpy(&indx_hdr,buff,sizeof(INDEX_HEADER));
    for(int i=0;i<4;i++)
    {
         if(str[i]!=indx_hdr.magicnumber[i])
         {
                                         
               printf(" Corrupted INDEEX HEADER ");
               return 0;
         }
    }
   // printf(" offset =%p ",indx_hdr.indxentryoff);
    //printf(" size of entry =%p ",indx_hdr.szofentry);
    offset=indx_hdr.indxentryoff+0x18;
    parse_index_entry(buff,offset,indx_hdr.szofentry);
             
    
    return 0;
    
    
}    
 
int read_cluster(u32 LCN,u32 len)
{
 
  
    u32 start_lba,start_sec,sector;
    int clus_sz;
    u8 far *buff;
       
    start_lba=fs_ptr->start_lba;
    clus_sz=fs_ptr->clus_sz;
    start_sec=mul32(LCN,clus_sz);
    sector=start_lba+start_sec;
    
    buff=(u8 far *)farmalloc(clus_sz*512);
   // printf(" buff =%p ",buff);
    for(u32 i=0;i<len;i++)
    {
         read_disk(fs_ptr->dptr,sector,clus_sz,buff);
         sector+=clus_sz;   
         parse_indexheader(buff);   
    }        
    
            
   // printf(" %p ",sector);
    
    return 0;
    
 
    
}
int copy_cluster(u8 far *buffer,int clus_sz)
{
    
    u32 size;
    if((file_size-cur_size)<(clus_sz*512))
     size=(file_size-cur_size);
    else
     size=clus_sz*512;
     
     u8 far *ptr=(file_addr+cur_size);
     for(int i=0;i<size;i++)
     {
          *ptr++=*buffer++;
     }  
     cur_size+=size;      
   //  printf(" copy size =%p ",size);
    return 0;
        
}    


int read_file_cluster(u32 LCN,u32 len)
{
 
    u32 start_lba,start_sec,sector;
    int clus_sz;
    u8 far *buff;
       
    start_lba=fs_ptr->start_lba;
    clus_sz=fs_ptr->clus_sz;
    start_sec=mul32(LCN,clus_sz);
    sector=start_lba+start_sec;
    
    buff=(u8 far *)farmalloc(clus_sz*512);
    
    for(u32 i=0;i<len;i++)
    {
         read_disk(fs_ptr->dptr,sector,clus_sz,buff);
         sector+=clus_sz;   
         copy_cluster(buff,clus_sz); 
         
            
    }        
    return 0;
    
}



int parse_datarun(int offset)
{
    
    u8 ch;
    int new_off=offset;
    int offsz,lensz;
    u32 c=0,clus=0,len=0;
    while(1)
    {
         memcpy(&ch,filebuff+new_off,1);
         new_off++;
         if(!ch)
           break;
                        
          offsz=(ch>>4);
          lensz=(ch&0x0f);
          
          len=0;
          clus=0;
         
         memcpy(&len,filebuff+new_off,lensz);
         new_off+=lensz;
         
         
         memcpy(&clus,filebuff+new_off,offsz);
         new_off+=offsz;
         
         /*
         	////// if the last bit of n64Offset is 1 then its -ve so u got to make it -ve /////
			if((((char*)&n64Offset)[chOffsetSz-1])&0x80)
				for(int i=sizeof(LONGLONG)-1;i>(chOffsetSz-1);i--)
					((char*)&n64Offset)[i] = 0xff;
			
         */
         
         if(((char *)&clus)[offsz-1]&0x80)
         {
             // printf(" -");
              for(int l=sizeof(u32)-1;l>(offsz-1);l--)
              ((char *)&clus)[l]=0xff; 
                          
         }
        
         c=c+clus;
        // printf(" C= %p len =%p ",c,len);
         read_cluster(c,len);
        
         
         
         
    }
    return 0;
    
}




int parse_file_datarun(int offset)
{
    
    u8 ch;
    int new_off=offset;
    int offsz,lensz;
    u32 c=0,clus=0,len=0;
       
    while(1)
    {
         memcpy(&ch,filebuff+new_off,1);
        
         if(!ch)
           break;
                        
          offsz=(ch>>4);
          lensz=(ch&0x0f);
                       
         //printf(" off=%x len=%x ",offsz,lensz);
         len=0;
         clus=0;
         new_off++;
         memcpy(&len,(filebuff+new_off),lensz);
         new_off+=lensz;
      
         memcpy(&clus,(filebuff+new_off),offsz);
         new_off+=offsz;
         
         if(((char *)&clus)[offsz-1]&0x80)
         {
             // printf(" -");
              for(int l=sizeof(u32)-1;l>(offsz-1);l--)
              ((char *)&clus)[l]=0xff; 
                          
         }
         c=c+clus;
         read_file_cluster(c,len);
         //printf("c= %p ",c);
         
    }
    return 0;
    

}




PENTRY make_ntfs_dtree(PFILESYSTEM ptr,PENTRY entry)
{
       
       FILE_RECORD far *file_rec;
       ATTRIBUTE_HEADER attrib_hdr;
       int attrib_offs;
       u32 mft=entry->mft;
       pfront=NULL;
       prear=NULL;
       parent=entry;
       fs_ptr=ptr;
       
     //  printf(" Mft =%p ",mft);
       if(open_file_record(ptr,mft)==FAILURE)
       {
           printf(" Cannot OPen FIle record ");
           return NULL;
       } 
       file_rec=(FILE_RECORD far *)filebuff;
       attrib_offs=file_rec->attrib_offset;
       
      //printf(" Attrib Offset =%x ",attrib_offs);
       
       memcpy(&attrib_hdr,filebuff+attrib_offs,sizeof(ATTRIBUTE_HEADER));
       while((attrib_hdr.type!=0)&&(attrib_hdr.type!=0xffffffff))
       {
              
           // printf("\n Attribute type= %x ",attrib_hdr.type);
            int offs1=attrib_hdr.resident.attroffs+attrib_offs;
            if(attrib_hdr.type==0x90)
            { 
                        
                parse_indexroot(offs1,attrib_hdr.resident.len);
              
            }
            else if(attrib_hdr.type==0xA0)
            {
                 
                  int datarun_offs=attrib_offs+attrib_hdr.non_resident.datarun;
                  parse_datarun(datarun_offs);
                
            } 
            attrib_offs+=attrib_hdr.len;                                            
            memcpy(&attrib_hdr,filebuff+attrib_offs,sizeof(attrib_hdr));          
       } 
       return pfront; 
       
}       

PENTRY open_ntfs_file(PFILESYSTEM ptr,PENTRY entry,u8 far *buffer)
{
       
       FILE_RECORD far *file_rec;
       ATTRIBUTE_HEADER attrib_hdr;
       int attrib_offs;
       u32 mft=entry->mft;
       
       file_addr=buffer;
       fs_ptr=ptr;
       file_size=entry->size;
       cur_size=0;
       
       
       if(open_file_record(ptr,mft)==FAILURE)
       {
           printf(" Cannot OPen FIle record ");
           return NULL;
       } 
       file_rec=(FILE_RECORD far *)filebuff;
       attrib_offs=file_rec->attrib_offset;
       
       
       memcpy(&attrib_hdr,filebuff+attrib_offs,sizeof(ATTRIBUTE_HEADER));
       while((attrib_hdr.type!=0)&&(attrib_hdr.type!=0xffffffff))
       {
              
           // printf("\n Attribute type= %x ",attrib_hdr.type);
           
            int offs1=attrib_hdr.resident.attroffs+attrib_offs;
            
            if(attrib_hdr.type==0x80)
            {
              //  printf(" non res flag =%x ",attrib_hdr.nonresflag);
                if(attrib_hdr.nonresflag)
                {
                    //  printf(" non resident ");                   
                      int datarun_offs=attrib_offs+attrib_hdr.non_resident.datarun;
                    //  printf(" data_run offs =%x ",datarun_offs);
                      parse_file_datarun(datarun_offs);
                       
                                         
                }
                else
                {
                    //resident data do nothing just copy
                    int data_offs=attrib_hdr.resident.attroffs+attrib_offs;
                    memcpy(buffer,filebuff+data_offs,entry->size);                   
                }                             
                                     
            }     
            attrib_offs+=attrib_hdr.len;                                            
            memcpy(&attrib_hdr,filebuff+attrib_offs,sizeof(attrib_hdr));          
       } 
       return pfront; 
       
}       



