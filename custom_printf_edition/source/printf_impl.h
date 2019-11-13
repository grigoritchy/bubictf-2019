#include <stdint.h>
#include <string.h>
#include <libio.h>

/*

libc.so.6's _IO_FILE for printf. we wrap it to use own structure first...

struct _IO_FILE {
  int _flags;		// High-order word is _IO_MAGIC; rest is flags.
#define _IO_file_flags _flags

  // The following pointers correspond to the C++ streambuf protocol. /
  // Note:  Tk uses the _IO_read_ptr and _IO_read_end fields directly. /
  char* _IO_read_ptr;	// Current read pointer /
  char* _IO_read_end;	// End of get area. /
  char* _IO_read_base;	// Start of putback+get area. /
  char* _IO_write_base;	// Start of put area. /
  char* _IO_write_ptr;	// Current put pointer. /
  char* _IO_write_end;	// End of put area. /
  char* _IO_buf_base;	// Start of reserve area. /
  char* _IO_buf_end;	// End of reserve area. /
  // The following fields are used to support backing up and undo. /
  char *_IO_save_base; // Pointer to start of non-current get area. /
  char *_IO_backup_base;  // Pointer to first valid character of backup area /
  char *_IO_save_end; // Pointer to end of non-current get area. /

  struct _IO_marker *_markers;

  struct _IO_FILE *_chain;

  int _fileno;
#if 0
  int _blksize;
#else
  int _flags2;
#endif
  _IO_off_t _old_offset; This used to be _offset but it's too small.

#define __HAVE_COLUMN // temporary 
  / 1+column number of pbase(); 0 is unknown.
  unsigned short _cur_column;
  signed char _vtable_offset;
  char _shortbuf[1];

  char* _save_gptr;  char* _save_egptr;

  _IO_lock_t *_lock;
#ifdef _IO_USE_OLD_IO_FILE
};
*/

#define BUFFER_SIZE 0x1000

#define NO_BUFFER 1 << 0
#define USE_BUFFER 1 << 1
#define REACH_BUFFER_EDGE 1 << 2
#define FLUSH_BUFFER 1 << 3
#define REUSE_BUFFER 1 << 4

#define EMOJI_START 0xf0

enum {
    CHECK_BUFFER,
    RESET_BUFFER,
} control_types;

enum {
  POUNTING,
  SCREAMING,
  THINKING,
  ZERO,
  ONE,
  TWO,
  THREE,
  FOUR,
  FIVE,
  SIX,
  SEVEN,
  EIGHT,
  NINE
} format_string_types;

struct vtable_io {
    struct _IO_FILE *link;
    int (*_IO_new_file_xsputn) (struct _IO_FILE *, const void *, _IO_size_t);
};

struct printf_io {
    uint64_t state;
    unsigned char *buf_base;
    unsigned char *buf_ptr;
    unsigned char *buf_end;
    struct vtable_io *vtable;
};


unsigned char *EMOJI_POUNTING;
unsigned char *EMOJI_JOY; 
unsigned char *EMOJI_THINKING;
unsigned char *EMOJI_ZERO;
unsigned char *EMOJI_ONE;
unsigned char *EMOJI_TWO;
unsigned char *EMOJI_THREE;
unsigned char *EMOJI_FOUR;
unsigned char *EMOJI_FIVE;
unsigned char *EMOJI_SIX;
unsigned char *EMOJI_SEVEN;
unsigned char *EMOJI_EIGHT;
unsigned char *EMOJI_NINE;

size_t switching_count;
unsigned char width;
unsigned char io_nobuf[8];
unsigned char *io_buf;

struct printf_io io;

int my_printf(const unsigned char *format, ...);
void switch_mode(int mode);
int io_buf_control(int ctl);
char* itoa(int i, char b[]);

void outstring(const unsigned char *outstring, size_t outstring_length);

__extern_always_inline const unsigned char *find_specb (const unsigned char *format)
{
  return (const unsigned char *) strchr ((const unsigned char *) format, EMOJI_START);
}

__extern_always_inline int emoji_compare(const unsigned char *buf) {
  if(buf[0] == EMOJI_THINKING[0]) {
    if(buf[1] == EMOJI_THINKING[1] && buf[2] == EMOJI_THINKING[2])
      return THINKING;
    else if(buf[1] == EMOJI_JOY[1] && buf[2] == EMOJI_JOY[2])
      return SCREAMING;
    else if(buf[1] == EMOJI_POUNTING[1] && buf[2] == EMOJI_POUNTING[2])
      return POUNTING;
  }

  return 0xff;
}



__extern_always_inline int number_compare(const unsigned char *buf) {
  if(buf[0] == EMOJI_ZERO[0] && buf[1] == EMOJI_ZERO[1] && buf[2] == EMOJI_ZERO[2]) {
    switch(buf[3]) {
      case 0x9b:
        return ZERO;
        break;
      case 0x90:
        return ONE;
        break;
      case 0x91:
        return TWO;
        break;
      case 0x92:
        return THREE;
        break;
      case 0x93:
        return FOUR;
        break;
      case 0x94:
        return FIVE;
        break;
      case 0x95:
        return SIX;
        break;
      case 0x96:
        return SEVEN;
        break;
      case 0x97:
        return EIGHT;
        break;
      case 0x98:
        return NINE;
        break;
      default:
        break;
      
    }
  }

  return 0xff;
}
