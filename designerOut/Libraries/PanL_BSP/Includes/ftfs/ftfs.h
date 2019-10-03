#ifndef _FT_FTFS_H_
#define _FT_FTFS_H_

#include <stdint.h>

#undef ENABLE_FIND    //to enable findfirst, findnext commands

#define NSECTORS 2048             // Size of flash in 4K sectors
                                  // For a  8MB flash: 2048
                                  // For a 64MB flash: 16384

#define PAGE_SIZE        256
#define PAGES_PER_SECTOR 16
#define SECTOR_SIZE      (PAGE_SIZE * PAGES_PER_SECTOR) 
#define SECTOR_DATA_SIZE (PAGE_SIZE * (PAGES_PER_SECTOR - 1))

#define MAX_NAME_LENGTH  223

#define ST_FILE   0xfc
#define ST_DIR    0xfd
#define ST_ANY    0xff

typedef struct
{
  char filename[244];
  uint32_t timestamp;
  uint32_t length;
  uint16_t type;
  uint16_t link;
}item_info_t;

struct Hfile {
  int mode;
  union {
    uint32_t length;    // opened "r": Length of file in bytes
    uint32_t dirloc;    // opened "w": directory entry
  };
  uint32_t pos;         // Current position in file
  uint16_t sector;      // Current sector
  uint16_t sector0;     // Origin sector
  union {
    uint8_t page[256];  // Cached page data
    uint16_t w[128];    // as words
    item_info_t dirent;
  };
};

int       ftfs_init(void);
int       ftfs_open(struct Hfile *f, const char* filename, int mode);
int      ftfs_close(struct Hfile *f);
int       ftfs_eof(struct Hfile *f);
uint32_t  ftfs_tell(struct Hfile *f);
uint32_t  ftfs_size(struct Hfile *f);
int       ftfs_getc(struct Hfile *f);
int       ftfs_seek(struct Hfile *f, uint32_t offset);
#ifdef __cplusplus
int       ftfs_read(struct Hfile *f, uint8_t *Buffer, int16_t bytetoread, int16_t &bytesread);
int       ftfs_write(struct Hfile *f, const uint8_t *Buffer, int16_t bytetowrite, int16_t &byteswritten);
#endif //__cplusplus
int       ftfs_putc(struct Hfile *f, char c);
int       ftfs_puts(struct Hfile *f, uint8_t *Buffer, int16_t buflen);
int       ftfs_gets(struct Hfile *f, uint8_t *Buffer, int16_t buflen);
int       ftfs_unlink(char* path);
int       ftfs_mkdir(char* dir_path);
int       ftfs_opendir(struct Hfile *dir, const char* dir_path);
int       ftfs_closedir(struct Hfile *dir);
int       ftfs_readdir(struct Hfile *dir, item_info_t *page);
int       ftfs_rename(char* old_path, char* new_path);
int       ftfs_fstat(char* path, char *page);
#ifdef ENABLE_FIND
int       ftfs_findfirst(struct Hfile *dir, item_info_t *page, const char *path, const char *pattern);
int       ftfs_findnext(struct Hfile *dir, item_info_t *page);
#endif

void ftfs_empty_root(void);
void ftfs_erase_eflash(void);



#define FTFS_ERROR_NOFILE              -3
#define FTFS_ERROR_NOPATH              -4
#define FTFS_ERROR_INVALIDFILE         -5
#define FTFS_ERROR_NOFILESYSTEM        -7
#define FTFS_ERROR_FILESYSTEMFULL      -8

#endif
