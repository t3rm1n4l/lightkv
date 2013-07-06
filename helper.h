#ifndef HELPER_H
#define HELPER 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logger.h"

#define DATAFILE_FORMATSTR  "data.%d.db"

#define LOCSTR "%"PRIu64" (%d:%d,%d)"
#define LOCPARAMS(x) x.val,x.l.num,x.l.offset,x.l.sclass

char *joinpath(const char *base, const char *next);
char *getfilepath(const char *base, int n) ;
void print_buf(const char *buf, int len) ;
inline void print_record(record *rec) ;
uint32_t roundsize(uint32_t v) ;
size_t get_val(record *r, char **v) ;
char *get_key(record *r) ;
int get_sizeslot(uint32_t v) ;
uint32_t get_slotsize(int slot);

char *joinpath(const char *base, const char *next) {
    size_t l1,l2;
    char *s;
    l1 = strlen(base);
    l2 = strlen(next);

    if (base[l1-1] == '/') {
        l1--;
    }

    s = (char *) malloc(l1+l2+2);
    strncpy(s, base, l1);
    strncpy(s+l1+1, next, l2);
    s[l1] = '/';
    s[l1+l2+2] = '\0';

    return s;
}

char *getfilepath(const char *base, int n) {
    char name[200];
    // TODO: Fail checks
    snprintf(name, 200, DATAFILE_FORMATSTR, n);
    return joinpath(base, name);
}

void print_buf(const char *buf, int len) {
    int i;
    printf("%p: ", buf);
    for (i=0; i < len; i++) {
        printf("%c", buf[i]);
    }
    printf("\n");
}

inline void print_record(record *rec) {
    char key[rec->extlen+1];
    char val[rec->len - rec->extlen - RECORD_HEADER_SIZE];
    strncpy(key, (char *) rec + RECORD_HEADER_SIZE, rec->extlen);
    key[rec->extlen] = '\0';
    strncpy(val, (char *) rec + RECORD_HEADER_SIZE + rec->extlen, sizeof(val));

    debug_log("Record: type:%d size:%d keylen:%d key:%s val:%s",
            rec->type,
            rec->len,
            rec->extlen,
            key,
            val);
}

uint32_t roundsize(uint32_t v) {
    // TODO: Add bound checks
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return v;
}

int get_sizeslot(uint32_t v) {
    // Check if power of two
    if (!(v&(v-1))) {
        v = roundsize(v);
    }

    int n = 0;
    while( v>>=1 ) n++;
    n = n - FIRST_SIZECLASS;
    if (n < 0) {
        n = 0;
    } else if (n > FIRST_SIZECLASS + MAX_SIZES - 1) {
        n = MAX_SIZES - 1;
    }

    return n;
}

uint32_t get_slotsize(int slot) {
    slot += FIRST_SIZECLASS;
    return pow(2, slot);
}

freeloc *freeloc_new(loc l) {
    freeloc *f = (freeloc *) malloc(sizeof(freeloc));
    f->l = l;
    f->next = NULL;
    f->prev = NULL;
    return f;
}

char *get_key(record *r) {
    int l = r->extlen;
    char *buf = (char *) malloc(l+1);
    memcpy(buf, (char *) r + RECORD_HEADER_SIZE, l);
    buf[l] = '\0';
    return buf;
}

size_t get_val(record *r, char **v) {
    int l = r->len - RECORD_HEADER_SIZE - r->extlen;
    char *buf = (char *) malloc(l);
    memcpy(buf, (char *) r + RECORD_HEADER_SIZE + r->extlen, l);
    *v = buf;
    return l;
}

#endif
