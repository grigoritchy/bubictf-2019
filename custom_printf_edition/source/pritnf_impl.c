#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include "printf_impl.h"


int my_printf(const unsigned char *format, ...) {
    size_t outstring_length = 0;
    unsigned char width_count;
    const unsigned char *lead_str_cur;
    const unsigned char *lead_str_end;
    _IO_va_list arg;
    bool format_found;
    bool width_found;
    char buf[0x20] = {0, };

    va_start(arg, format);

    lead_str_cur = lead_str_end = find_specb(format);
    if(lead_str_end)
        outstring_length = lead_str_end - format;
    else
        outstring_length = strlen(format);

    if(outstring_length) {
        width += outstring_length;
        outstring(format, outstring_length);
    }

    if(lead_str_cur == NULL || *lead_str_cur == 0)
        goto done;

    width = 0;
    do {
        width_count = 0;

        do {
            width_found = true;
            size_t result = number_compare(lead_str_cur);
            size_t retval = (width_count << 4);
            if(retval == 0)
                retval = 1;

            switch(result) {
                case ZERO: {
                    width += 0 * retval;
                    lead_str_cur += 4;
                    break;
                }
                case ONE: {
                    width += 1 * retval;
                    lead_str_cur += 4;
                    break;
                }
                case TWO: {
                    width += 2 * retval;
                    lead_str_cur += 4;
                    break;
                }
                case THREE: {
                    width += 3 * retval;
                    lead_str_cur += 4;
                    break;
                }
                case FOUR: {
                    width += 4 * retval;
                    lead_str_cur += 4;
                    break;
                }
                case FIVE: {
                    width += 5 * retval;
                    lead_str_cur += 4;
                    break;
                }
                case SIX: {
                    width += 6 * retval;
                    lead_str_cur += 4;
                    break;
                }
                case SEVEN: {
                    width += 7 * retval;
                    lead_str_cur += 4;
                    break;
                }
                case EIGHT: {
                    width += 8 * retval;
                    lead_str_cur += 4;
                    break;
                }
                case NINE: {
                    width += 9 * retval;
                    lead_str_cur += 4;
                    break;
                }
                default: {
                    width_found = false;
                    break;
                }
            }

            if(width_found == false) break;
            if(width_count == 1) break;
            width_count++;
        } while(1);

        if(*lead_str_cur++ == EMOJI_START) {
            format_found = true;
            
            switch(emoji_compare(lead_str_cur)) {
                case POUNTING:{
                    unsigned char *ptr = va_arg(arg, void *);
                    *ptr = width;
                    lead_str_cur += 3;
                    break;
                }
                case SCREAMING: {
                    char *ptr = va_arg(arg, void *);
                    outstring(ptr, strlen(ptr));
                    lead_str_cur += 3;
                    break;
                }
                case THINKING:{
                    int val = va_arg(arg, int);
                    itoa(val, &buf);
                    outstring(buf, strlen(buf));
                    lead_str_cur += 3;
                    break;
                }
                default: {
                    format_found = false;
                    break;
                }
            }
        }

        if(*lead_str_cur == 0)
            goto done;

        lead_str_end = find_specb(lead_str_cur);
            
        if(!format_found) lead_str_cur--;

        if(lead_str_end)
            outstring_length = lead_str_end - lead_str_cur;
        else
            outstring_length = strlen(lead_str_cur);
        
        width += outstring_length;
        outstring(lead_str_cur, outstring_length);

        lead_str_cur += outstring_length;

        if(*lead_str_cur == 0)
            goto done;

    } while(1);
done:
    va_end(arg);

    return 0;
}


void outstring(const unsigned char *outstring, size_t outstring_length) {
    unsigned char c;
    struct _IO_FILE *link = io.vtable->link;

    if(outstring_length > (BUFFER_SIZE / 2)) {
        switch_mode(USE_BUFFER);
        for(int i = 0; i < outstring_length; i++) {
            c = outstring[i];
            *io.buf_ptr++ = c;

            if(io_buf_control(CHECK_BUFFER) & REACH_BUFFER_EDGE ) {
                io.state |= FLUSH_BUFFER;
                io.vtable->_IO_new_file_xsputn(link, io.buf_base, io.buf_ptr - io.buf_base);
                io_buf_control(RESET_BUFFER);
            }
        }

        if( !(io.state & FLUSH_BUFFER)) {
            io.vtable->_IO_new_file_xsputn(link, io.buf_base, io.buf_ptr - io.buf_base);
            io_buf_control(RESET_BUFFER);
        }

        io.state &= ~(FLUSH_BUFFER);
    }
    else {
        switch_mode(NO_BUFFER);
        for(int i = 0; i < outstring_length; i++) {
            c = outstring[i];
            *io.buf_ptr++ = c; 
            if(io_buf_control(CHECK_BUFFER) & REACH_BUFFER_EDGE ) {
                io_buf_control(RESET_BUFFER);
                io.vtable->_IO_new_file_xsputn(link, io.buf_ptr, io.buf_end - io.buf_ptr);
            }
        }

    }

}

void switch_mode(int mode) {
    switch(mode) {
        case NO_BUFFER: {
            if(io.state & NO_BUFFER)
                break;

            if( (io_buf != NULL && io.state & USE_BUFFER) && !(io.state & REUSE_BUFFER)) {
                free(io_buf);
            }
            

            io.state = NO_BUFFER;
            io.buf_base = io_nobuf;
            io.buf_ptr = io_nobuf;
            io.buf_end = (uint64_t)io_nobuf + (uint8_t)1;
            break;
        }
        case USE_BUFFER: {
            if(io.state & USE_BUFFER)
                break;

            if(!io_buf){
                io_buf = (unsigned char *)malloc(BUFFER_SIZE);
                memset(io_buf, 0, BUFFER_SIZE);
            }
            
            io.state = USE_BUFFER;
            if(switching_count > 0)
                io.state |= REUSE_BUFFER;
                
            io.buf_base = io_buf;
            io.buf_ptr = io_buf;
            io.buf_end = io_buf + BUFFER_SIZE;
            switching_count++;
            break;
        }
        default:
            break;
    }
}

int io_buf_control(int ctl) {
    switch(ctl) {
        case CHECK_BUFFER: {
            if(io.buf_ptr == io.buf_end) {
                return REACH_BUFFER_EDGE;
            }
            break;
        }
        case RESET_BUFFER: {
            io.buf_ptr = io.buf_base;
            break;
        }
        default:
            break;
    }
    
    return 0;
}

char* itoa(int i, char b[]){
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do {
        ++p;
        shifter = shifter/10;
    } while(shifter);
    *p = '\0';
    do{
        *--p = digit[i%10];
        i = i/10;
    } while(i);
    return b;
}