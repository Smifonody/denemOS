#include "disk.h"

/* ===== PORT I/O ===== */

internal void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

internal uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

internal uint16_t inw(uint16_t port) {
    uint16_t ret;
    __asm__ volatile ("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

internal void outw(uint16_t port, uint16_t val) {
    __asm__ volatile ("outw %0, %1" : : "a"(val), "Nd"(port));
}

/* ===== ATA YARDIMCI ===== */

internal void ata_wait() {
    while (inb(ATA_PORT_CMD) & ATA_STATUS_BSY);
}

internal void ata_setup(disk *dd, int32 lba) {
    uint8_t drive_sel = 0xE0 | (dd->drive & 0x01) << 4 | ((lba >> 24) & 0x0F);
    outb(ATA_PORT_DRIVE,   drive_sel);
    outb(ATA_PORT_SECTORS, 1);
    outb(ATA_PORT_LBA_LO,  (uint8_t)(lba));
    outb(ATA_PORT_LBA_MID, (uint8_t)(lba >> 8));
    outb(ATA_PORT_LBA_HI,  (uint8_t)(lba >> 16));
}

/* ===== DISK FONKSİYONLARI ===== */

internal disk drives[2];
internal int8 drive_count = 0;

void dinit() {
    drive_count = 0;
}

disk *dattach(int8 drive) {
    if (drive_count >= 2) return 0;

    drives[drive_count].drive         = drive;
    drives[drive_count].total_sectors = 0;

    drive_count++;
    return &drives[drive_count - 1];
}

void ddetach(disk *dd) {
    if (!dd) return;
    dd->drive         = 0;
    dd->total_sectors = 0;
}

void dread(disk *dd, int32 lba, void *buf) {
    int16 i;
    uint16_t *ptr = (uint16_t *)buf;

    ata_wait();
    ata_setup(dd, lba);
    outb(ATA_PORT_CMD, ATA_CMD_READ);
    ata_wait();

    for (i = 0; i < 256; i++) {
        ptr[i] = inw(ATA_PORT_DATA);
    }
}

void dwrite(disk *dd, int32 lba, void *buf) {
    int16 i;
    uint16_t *ptr = (uint16_t *)buf;

    ata_wait();
    ata_setup(dd, lba);
    outb(ATA_PORT_CMD, ATA_CMD_WRITE);
    ata_wait();

    for (i = 0; i < 256; i++) {
        outw(ATA_PORT_DATA, ptr[i]);
    }

    outb(ATA_PORT_CMD, ATA_CMD_FLUSH);
    ata_wait();
}