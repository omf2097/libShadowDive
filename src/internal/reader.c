#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>

#include "shadowdive/internal/reader.h"

struct sd_reader_t {
    FILE *handle;
    long filesize;
};

sd_reader* sd_reader_open(const char *file) {
    sd_reader *reader = malloc(sizeof(const sd_reader));

    // Attempt to open file (note: Binary mode!)
    reader->handle = fopen(file, "rb");
    if(!reader->handle) {
        free(reader);
        return 0;
    }

    // Find file size
    fseek(reader->handle, 0, SEEK_END);
    reader->filesize = ftell(reader->handle);
    fseek(reader->handle, 0, SEEK_SET);

    // All done.
    return reader;
}

long sd_reader_filesize(const sd_reader *reader) {
    return reader->filesize;
}

void sd_reader_close(sd_reader *reader) {
    fclose(reader->handle);
    free(reader);
}

int sd_reader_set(const sd_reader *reader, long offset) {
    return fseek(reader->handle, offset, SEEK_SET);
}

int sd_reader_ok(const sd_reader *reader) {
    if(feof(reader->handle)) {
        return 0;
    }
    return 1;
}

long sd_reader_pos(const sd_reader *reader) {
    return ftell(reader->handle);
}

int sd_read_buf(const sd_reader *reader, char *buf, int len) {
    if(fread(buf, 1, len, reader->handle) != len) {
        return 0;
    }
    return 1;
}

int sd_peek_buf(const sd_reader *reader, char *buf, int len) {
    if(sd_read_buf(reader, buf, len)) {
        return 0;
    }
    fseek(reader->handle, ftell(reader->handle) - len, SEEK_SET);
    return 1;
}

uint8_t sd_read_ubyte(const sd_reader *reader) {
    uint8_t d;
    sd_read_buf(reader, (char*)&d, 1);
    return d;
}

uint16_t sd_read_uword(const sd_reader *reader) {
    uint16_t d;
    sd_read_buf(reader, (char*)&d, 2);
    return d;
}

uint32_t sd_read_udword(const sd_reader *reader) {
    uint32_t d;
    sd_read_buf(reader, (char*)&d, 4);
    return d;
}

int8_t sd_read_byte(const sd_reader *reader) {
    int8_t d;
    sd_read_buf(reader, (char*)&d, 1);
    return d;
}

int16_t sd_read_word(const sd_reader *reader) {
    int16_t d;
    sd_read_buf(reader, (char*)&d, 2);
    return d;
}

int32_t sd_read_dword(const sd_reader *reader) {
    int32_t d;
    sd_read_buf(reader, (char*)&d, 4);
    return d;
}

float sd_read_float(const sd_reader *reader) {
    float f;
    sd_read_buf(reader, (char*)&f, 4);
    return f;
}

uint8_t sd_peek_ubyte(const sd_reader *reader) {
    uint8_t d;
    sd_peek_buf(reader, (char*)&d, 1);
    return d;
}

uint16_t sd_peek_uword(const sd_reader *reader) {
    uint16_t d;
    sd_peek_buf(reader, (char*)&d, 2);
    return d;
}

uint32_t sd_peek_udword(const sd_reader *reader) {
    uint32_t d;
    sd_peek_buf(reader, (char*)&d, 4);
    return d;
}

int8_t sd_peek_byte(const sd_reader *reader) {
    int8_t d;
    sd_peek_buf(reader, (char*)&d, 1);
    return d;
}

int16_t sd_peek_word(const sd_reader *reader) {
    int16_t d;
    sd_peek_buf(reader, (char*)&d, 2);
    return d;
}

int32_t sd_peek_dword(const sd_reader *reader) {
    int32_t d;
    sd_peek_buf(reader, (char*)&d, 4);
    return d;
}

float sd_peek_float(const sd_reader *reader) {
    float f;
    sd_peek_buf(reader, (char*)&f, 4);
    return f;
}

int sd_match(const sd_reader *reader, char *buf, unsigned int nbytes) {
    char t[nbytes];
    sd_peek_buf(reader, t, nbytes);
    if(memcmp(t, buf, nbytes) == 0) {
        return 1;
    }
    return 0;
}

void sd_skip(const sd_reader *reader, unsigned int nbytes) {
    fseek(reader->handle, nbytes, SEEK_CUR);
}

int sd_read_scan(const sd_reader *reader, const char* format, ...) {
    va_list argp;
    va_start(argp, format);
    int ret = fscanf(reader->handle, format, argp);
    va_end(argp);
    return ret;
}

int sd_read_line(const sd_reader *reader, char *buffer, int maxlen) {
    if(fgets(buffer, maxlen, reader->handle) == NULL) {
        return 1;
    }
    return 0;
}

char* sd_read_variable_str(const sd_reader *r) {
    uint16_t len = sd_read_uword(r);
    char *str = NULL;
    if(len > 0) {
        str = (char*)malloc(len);
        sd_read_buf(r, str, len);
    }
    return str;
}
