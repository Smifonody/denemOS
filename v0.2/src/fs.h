#ifndef FS_H
#define FS_H

#include "disk.h"

/* FS imzası */
#define FS_MAGIC 0xDEADB00B

/* Sektör düzeni */
#define FS_SECTOR_SUPERBLOCK  0
#define FS_SECTOR_INODE_BMP   1
#define FS_SECTOR_BLOCK_BMP   2
#define FS_SECTOR_INODE_TABLE 3
#define FS_SECTOR_DATA        11

/* Limitler */
#define FS_MAX_INODES  64
#define FS_MAX_BLOCKS  512
#define FS_NAME_LEN    32
#define FS_BLOCK_SIZE  512

/* Superblock */
struct packed s_superblock {
    int32 magic;
    int32 total_sectors;
    int32 inode_count;
    int32 data_start;
};
typedef struct s_superblock superblock;

/* Inode */
struct packed s_inode {
    int8  used;
    int8  name[FS_NAME_LEN];
    int32 size;
    int32 blocks[8];
};
typedef struct s_inode inode;

/* Fonksiyonlar */
void fs_format(disk *dd);
void fs_mount(disk *dd);
int8 fs_create(disk *dd, int8 *name);
int8 fs_write(disk *dd, int8 *name, void *buf, int32 size);
int8 fs_read(disk *dd, int8 *name, void *buf);
int8 fs_delete(disk *dd, int8 *name);

#endif