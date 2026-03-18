#include "fs.h"

internal disk *mounted_disk = 0;

/* ===== YARDIMCI FONKSİYONLAR ===== */

internal void memset8(void *ptr, int8 val, int32 size) {
    int32 i;
    int8 *p = (int8 *)ptr;
    for (i = 0; i < size; i++) p[i] = val;
}

internal void memcpy8(void *dst, void *src, int32 size) {
    int32 i;
    int8 *d = (int8 *)dst;
    int8 *s = (int8 *)src;
    for (i = 0; i < size; i++) d[i] = s[i];
}

internal int8 strcmp8(int8 *a, int8 *b) {
    while (*a && *b && *a == *b) { a++; b++; }
    return *a - *b;
}

internal void strcpy8(int8 *dst, int8 *src) {
    while (*src) { *dst++ = *src++; }
    *dst = 0;
}

/* ===== BITMAP ===== */

internal void bmp_set(uint8_t *bmp, int32 i) {
    bmp[i / 8] |= (1 << (i % 8));
}

internal void bmp_clear(uint8_t *bmp, int32 i) {
    bmp[i / 8] &= ~(1 << (i % 8));
}

int8 bmp_get(uint8_t *bmp, int32 i) {
    return (bmp[i / 8] >> (i % 8)) & 1;
}

internal int32 bmp_find_free(uint8_t *bmp, int32 max) {
    int32 i;
    for (i = 0; i < max; i++) {
        if (!bmp_get(bmp, i)) return i;
    }
    return -1;
}

/* ===== FORMAT ===== */

void fs_format(disk *dd) {
    uint8_t buf[512];

    /* superblock yaz */
    superblock *sb = (superblock *)buf;
    memset8(buf, 0, 512);
    sb->magic         = FS_MAGIC;
    sb->total_sectors = dd->total_sectors;
    sb->inode_count   = FS_MAX_INODES;
    sb->data_start    = FS_SECTOR_DATA;
    dwrite(dd, FS_SECTOR_SUPERBLOCK, buf);

    /* inode bitmap temizle */
    memset8(buf, 0, 512);
    dwrite(dd, FS_SECTOR_INODE_BMP, buf);

    /* block bitmap temizle */
    memset8(buf, 0, 512);
    dwrite(dd, FS_SECTOR_BLOCK_BMP, buf);

    /* inode tablosunu temizle */
    int32 i;
    for (i = 0; i < 8; i++) {
        memset8(buf, 0, 512);
        dwrite(dd, FS_SECTOR_INODE_TABLE + i, buf);
    }
}

/* ===== MOUNT ===== */

void fs_mount(disk *dd) {
    uint8_t buf[512];
    dread(dd, FS_SECTOR_SUPERBLOCK, buf);
    superblock *sb = (superblock *)buf;

    if (sb->magic != FS_MAGIC) {
        fs_format(dd);
    }

    mounted_disk = dd;
}

/* ===== INODE OKUMA/YAZMA ===== */

void inode_read(int32 idx, inode *out) {
    uint8_t buf[512];
    int32 sector = FS_SECTOR_INODE_TABLE + (idx / 4);
    int32 offset = (idx % 4) * sizeof(inode);
    dread(mounted_disk, sector, buf);
    memcpy8(out, buf + offset, sizeof(inode));
}

internal void inode_write(int32 idx, inode *in) {
    uint8_t buf[512];
    int32 sector = FS_SECTOR_INODE_TABLE + (idx / 4);
    int32 offset = (idx % 4) * sizeof(inode);
    dread(mounted_disk, sector, buf);
    memcpy8(buf + offset, in, sizeof(inode));
    dwrite(mounted_disk, sector, buf);
}

/* ===== DOSYA OLUŞTUR ===== */

int8 fs_create(disk *dd, int8 *name) {
    uint8_t bmp[512];
    dread(dd, FS_SECTOR_INODE_BMP, bmp);

    int32 idx = bmp_find_free(bmp, FS_MAX_INODES);
    if (idx < 0) return -1;

    inode nd;
    memset8(&nd, 0, sizeof(inode));
    nd.used = 1;
    strcpy8(nd.name, name);
    nd.size = 0;

    bmp_set(bmp, idx);
    dwrite(dd, FS_SECTOR_INODE_BMP, bmp);
    inode_write(idx, &nd);

    return 0;
}

/* ===== DOSYAYA YAZ ===== */

int8 fs_write(disk *dd, int8 *name, void *buf, int32 size) {
    uint8_t ibmp[512], bbmp[512];
    dread(dd, FS_SECTOR_INODE_BMP, ibmp);
    dread(dd, FS_SECTOR_BLOCK_BMP, bbmp);

    /* inodu bul */
    int32 i;
    inode nd;
    int32 idx = -1;
    for (i = 0; i < FS_MAX_INODES; i++) {
        if (!bmp_get(ibmp, i)) continue;
        inode_read(i, &nd);
        if (strcmp8(nd.name, name) == 0) { idx = i; break; }
    }
    if (idx < 0) return -1;

    /* veriyi blocklara yaz */
    int8 *src = (int8 *)buf;
    int32 written = 0;
    int32 block_idx = 0;

    while (written < size && block_idx < 8) {
        int32 bfree = bmp_find_free(bbmp, FS_MAX_BLOCKS);
        if (bfree < 0) return -1;

        uint8_t block[512];
        memset8(block, 0, 512);

        int32 chunk = size - written;
        if (chunk > 512) chunk = 512;

        memcpy8(block, src + written, chunk);
        dwrite(dd, FS_SECTOR_DATA + bfree, block);

        bmp_set(bbmp, bfree);
        nd.blocks[block_idx++] = bfree;
        written += chunk;
    }

    nd.size = size;
    inode_write(idx, &nd);
    dwrite(dd, FS_SECTOR_BLOCK_BMP, bbmp);

    return 0;
}

/* ===== DOSYADAN OKU ===== */

int8 fs_read(disk *dd, int8 *name, void *buf) {
    uint8_t ibmp[512];
    dread(dd, FS_SECTOR_INODE_BMP, ibmp);

    int32 i;
    inode nd;
    int32 idx = -1;
    for (i = 0; i < FS_MAX_INODES; i++) {
        if (!bmp_get(ibmp, i)) continue;
        inode_read(i, &nd);
        if (strcmp8(nd.name, name) == 0) { idx = i; break; }
    }
    if (idx < 0) return -1;

    int8 *dst = (int8 *)buf;
    int32 read = 0;
    int32 block_idx = 0;

    while (read < nd.size && block_idx < 8) {
        uint8_t block[512];
        dread(dd, FS_SECTOR_DATA + nd.blocks[block_idx], block);

        int32 chunk = nd.size - read;
        if (chunk > 512) chunk = 512;

        memcpy8(dst + read, block, chunk);
        read += chunk;
        block_idx++;
    }

    return 0;
}

/* ===== DOSYA SİL ===== */

int8 fs_delete(disk *dd, int8 *name) {
    uint8_t ibmp[512], bbmp[512];
    dread(dd, FS_SECTOR_INODE_BMP, ibmp);
    dread(dd, FS_SECTOR_BLOCK_BMP, bbmp);

    int32 i;
    inode nd;
    int32 idx = -1;
    for (i = 0; i < FS_MAX_INODES; i++) {
        if (!bmp_get(ibmp, i)) continue;
        inode_read(i, &nd);
        if (strcmp8(nd.name, name) == 0) { idx = i; break; }
    }
    if (idx < 0) return -1;

    /* blocları serbest bırak */
    for (i = 0; i < 8; i++) {
        if (nd.blocks[i]) bmp_clear(bbmp, nd.blocks[i]);
    }

    bmp_clear(ibmp, idx);
    dwrite(dd, FS_SECTOR_INODE_BMP, ibmp);
    dwrite(dd, FS_SECTOR_BLOCK_BMP, bbmp);

    return 0;
}