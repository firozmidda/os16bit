
#include<type.h>
#ifndef _PARTITION_H_
 #define _PARTITION_H_
 
 
struct partition
{
    u8 bootable;            // Boot indicator. 0x80 for the  active partition and 0 for others
    u8 startside;            // Side where the partition begins
    u16 startsecCyl;          // The low 6 bits are the sector where the partition begins. The high 2 bits are the 2 high bits of the track where the partition begins.
    u8 parttype;             // 0 - Unused partition   1 - DOS with 12-bit FAT   2 - Xenix partition   4 - DOS with 16-bit FAT (smaller than 32 mb) 5 - Extented partition   6 - 16 bit (larger than 32 mb)   0xB - 32 bit upto 2048 mb 0xC- same as 0xB but used LBA1 int 13 extensions 0xE - same as 6 but uses LBA1 int 13 extensions 0xF - Same as 5 but uses LBA1 int 13 extensions */
    u8 endSide;        	   // Low 8 bits of the track where the partitions begins.
    u16 endSecCyl;            // The low 6 bits are the sector where the partition ends. The high 2 bits are the 2 high bits of the track where the partition ends.
    u32 partitionBegining;  // This is very usefull when you calculate logical cluster number in FAT. Just add this number with the corresponding FATEntry's Cluster Number
    u32 numberOfSectors;    // Number of sectors in the partition
};

struct _MBR
{
    char code[446];
    struct partition part[4];
    u16 LastTwo;
};


#define ACTIVE_FLAG	0x80	/* value for active in bootind field (hd0) */
#define NR_PARTITIONS	4	/* number of entries in partition table */
#define	PART_TABLE_OFF	   0x1BE	/* offset of partition table in boot sector */
#define BYTE_PER_SEC_OFF   0xB
#define CLUS_SZ_OFF        0xD
#define RES_SEC_OFF        0xE
#define FAT_NR_OFF         0x10
#define ROOT_SIZE_OFF      0x11
#define SEC_PER_FAT16_OFF  0x16    
#define SEC_PER_FAT32_OFF  0x24
#define ROOT_DIR_OFF       0x2c  

#define NTFS_MFT_OFF       0x30
     
#define FAT_DATA_SEC      


/* Partition types. */

#define NO_PART		0x00	/* unused entry */
#define FAT12       0x01     /* FAT 12 */      
#define FAT32_1     0x0b    /* fat type 1 */
#define FAT32_2     0x0c    /* fat type 2 */
#define NTFS        0x07    /* ntfs type  */
#define EXTEND_1    0x05    /* extend type 1 */
#define EXTEND_2    0x0f    /* extend type 2 */


 
 #endif  
