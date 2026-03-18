#include "../src/fs.h"

void ls(disk *dd) {
    // inode tablosunu tara, dosya isimlerini yaz
    int32 i;
    uint8_t ibmp[512];
    inode nd;

    dread(dd, FS_SECTOR_INODE_BMP, ibmp);

    for (i = 0; i < FS_MAX_INODES; i++) {
        if (!bmp_get(ibmp, i)) continue;
        inode_read(i, &nd);
        print((const char *)nd.name);
        print("\n");
    }
}