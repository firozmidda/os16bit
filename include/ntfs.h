
#include<type.h>
 #ifndef _NTFS_H_
 #define _NTFS_H_
 #include<fs.h>


struct _NTFS
{
       char jmp[3];
       char signature[8];
       u16 byte_per_sec;
       u8 sec_per_clus;
       u16 res1;
       char res2[3];
       u16 not_used;
       u8 media_desc;
       u16 res3;
       u16 sec_per_track;
       u16 number_head;
       u32 hidden_sec;
       u32 not_use2;
       u32 not_use3;
       u32 total_sec[2];
       u32 mft_start[2];
       u32 mft_mirr[2];
       u32 clus_per_frs;
       u32 clus_per_ind;
       u32 serial_no[2];
       u32 CRC;
       char code[426];
       u16 last_two;
};   


struct FILE_RECORD
{
       
      char file_signature[4];
      int fixupoffset;
      int fixupsize;
      u32 logseqnumber[2];
      u16 sequence;
      u16 hardlink;
      u16 attrib_offset;
      u16 flags;
      u32 reclen;
      u32 alloclen;
      u32 basemftrec[2];
      u16 nextattrid;
      u16 fixuppattern;
      u32 mftrecnumber;
};  


struct ATTR_FILE_NAME
{
	u32	ParentRef[2];		// File reference to the parent directory
	u32	CreateTime[2];		// File creation time
	u32	AlterTime[2];		// File altered time
	u32	MFTTime[2];		// MFT changed time
	u32	ReadTime[2];		// File read time
	u32	AllocSize[2];		// Allocated size of the file
	u32	RealSize[2];		// Real size of the file
	u32		Flags;			// Flags
	u32		ER;				// Used by EAs and Reparse
	u8		NameLength;		// Filename length in characters
	u8		NameSpace;		// Filename space
	u16		Name[1];		// Filename
};



struct ATTRIBUTE_HEADER
{
     u32 type;
     u32 len;
     u8 nonresflag;
     u8 namelen;
     u16 nameoffs;
     u16 flag;
     u16 id;
     union
     {
          struct RESIDENT
          {
               u32 len;
               u16 attroffs;
               u8 indexedflag;
               u8 padding;
          }resident;
          struct NON_RESIDENT
          {
                u32 startvcn[2];
                u32 lastvcn[2];
                u16 datarun;
                u16 compressionsize;
                u32 padding;
                u32 allocsize[2];
                u32 realsize[2];
                u32 streamsize[2];
          }non_resident;
     };
}; 

struct INDEX_ROOT
{
     // index root header  
     u32 attrib_type;
     u32 collation;
     u32 szofindxallocation;
     u8 clus_perindxrec;
     char padding[3];
     
     //index header
     
     u32 entry_offs;
     u32 szofindx_entry;
     u32 alloc_size;
     u8 flag;
     char padding2[3];
       
};

struct INDEX_HEADER
{
     char magicnumber[4];
     u16 updateseqoff;
     u16 szofupdateseq;
     u32 logfilseq[2];
     u32 vcnindx[2];
     u32 indxentryoff;
     u32 szofentry;
     u32 szofentryalloc;
     u8 flags;
     u8 padding[3];
     u16 update_seq;
};

struct INDEX_ENTRY
{
	u32	FileReference[2];	// Low 6B: MFT record index, High 2B: MFT record sequence number
	u16	Size;			// Length of the index entry
	u16	StreamSize;		// Length of the stream
	u16	Flags;			// Flags
	u8	Padding[2];		// Padding
	u8	Stream[1];		// Stream
	// VCN of the sub node in Index Allocation, Offset = Size - 8
};

  

 PENTRY make_ntfs_dtree(PFILESYSTEM ptr,PENTRY entry);
 PENTRY open_ntfs_file(PFILESYSTEM ptr,PENTRY entry,u8 far *buffer);
 
 #endif   
