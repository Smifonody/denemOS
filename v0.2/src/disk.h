/* disk.h */
#ifndef DISK_H
#define DISK_H
#include <stdint.h>
#include <stddef.h>

#define internal static
#define packed __attribute__((packed))

typedef int8_t  int8;
typedef int16_t int16;
typedef int32_t int32;

/* ATA portları, struct, fonksiyonlar... */

/* Sürücü seçimi */
#define DRIVE_MASTER 0x00
#define DRIVE_SLAVE  0x01

/* ATA portları */
#define ATA_PORT_DATA    0x1F0
#define ATA_PORT_ERROR   0x1F1
#define ATA_PORT_SECTORS 0x1F2
#define ATA_PORT_LBA_LO  0x1F3
#define ATA_PORT_LBA_MID 0x1F4
#define ATA_PORT_LBA_HI  0x1F5
#define ATA_PORT_DRIVE   0x1F6
#define ATA_PORT_CMD     0x1F7

/* ATA komutları */
#define ATA_CMD_READ  0x20
#define ATA_CMD_WRITE 0x30
#define ATA_CMD_FLUSH 0xE7

/* Status bitleri */
#define ATA_STATUS_BSY 0x80
#define ATA_STATUS_DRQ 0x08
#define ATA_STATUS_ERR 0x01

/* Disk struct */
struct packed s_disk {
    int8  drive;
    int32 total_sectors;
};
typedef struct s_disk disk;

/* Fonksiyonlar */
void  dinit(void);
disk *dattach(int8 drive);
void  ddetach(disk *dd);
void  dread(disk *dd, int32 lba, void *buf);
void  dwrite(disk *dd, int32 lba, void *buf);

#endif