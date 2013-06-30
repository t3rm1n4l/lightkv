#include "lightkv.h"
#include <sys/mman.h> /* mmap inside */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> /* file open modes and stuff */
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>


static uint32_t get_sizeslot(uint32_t v) {
    uint32_t n;
    // TODO: Add bound checks
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;

    while( v>>=1 ) n++;
    return n;
}

static freeloc *freeloc_new(record *r) {
    freeloc *l = malloc(sizeof(freeloc));
    l->size = r->len;
    l->next = NULL;
    l->prev = NULL;
}

static freeloc *freelist_add(freeloc *head, freeloc *n) {
    if (head) {
        head->prev = n;
        n->next = head;
    }

    return n;
}

static freeloc *freelist_get(freeloc *head, uint32_t size) {
    freeloc *f = NULL;
    uint32_t diff;

    while (head) {
        if (f == NULL) {
            f = head;
            diff = head->size - size;
        } else {
            uint32_t t;
            t = head->size - size;
            if (t < diff) {
                diff = t;
                f = head;
            }
        }
    }

    return f;
}

static freeloc *freelist_remove(freeloc *head, freeloc *f) {
    if (f) {
        if (f == head) {
            f->next->prev = NULL;
            head = f;
        } else {
            freeloc *tmp;
            tmp = f->next;
            tmp->prev = f->prev;
            f->prev->next = tmp;
        }

        free(f);
    }

    return head;
}

static int alloc_file(char *filepath, size_t size) {
    int fd;
    int rv;

    fd = open(filepath, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return fd;
    }

    // Fill the file with zeros
    lseek (fd, size-2, SEEK_SET);
    if ((rv = write(fd, "",1)) < 0) {
        return rv;
    }

    return 0;
}

static int map_file(void **map, char *filepath) {
    int fd;
    struct stat st;
    fd = open(filepath, O_RDWR);
    if (fd < 0) {
        return fd;
    }

    errno = 0;
    if (fstat(fd, &st) < 0) {
        return errno;
    }

    errno = 0;
    *map = mmap(0, st.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (errno < 0) {
        return errno;
    }

    close(fd);
    return 0;
}


/*
static loc create_nextloc(lightkv *kv, uint32_t size) {
   if ((kv->end_loc).offset + size + 2 > MAX_FILESIZE) {

   }

}
*/


int lightkv_init(lightkv **kv, char *base, bool prealloc) {
    // TODO: Add sanity checks

    *kv = malloc(sizeof(lightkv));
    assert(*kv > 0);

    (*kv)->prealloc = prealloc;
    (*kv)->basepath = base;
    (*kv)->filemaps[0] = NULL;
    (*kv)->nfiles = 1;

    char *f = getfilepath(base, 0);
    alloc_file(f, MAX_FILESIZE);

    if (map_file(&(*kv)->filemaps[0], f) < 0) {
        assert(false);
    }

    free(f);

    return 0;
}

uint64_t lightkv_insert(lightkv *kv, char *key, char *val, uint32_t len) {
    size_t size, keylen;
    freeloc *f;
    loc diskloc;
    record *rec;
    keylen = strlen(key);
    size = RECORD_HEADER_SIZE + keylen + len;

    rec = malloc(size);
    rec->type = RECORD_VAL ;
    rec->len = size;
    rec->extlen = keylen;
    int slot = get_sizeslot(size);
    f = freelist_get(kv->freelist[slot], size);
    if (f) {
        diskloc = f->l;
        kv->freelist[slot] = freelist_remove(kv->freelist[slot], f);
    } else {
        ;
//        diskloc = create_nextloc(kv, kv->end_loc);
    }

}

main() {
    lightkv *kv;
    lightkv_init(&kv, "/tmp/", true);
}
