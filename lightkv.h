#ifndef LIGHTKV_H
#define LIGHTKV_H 1

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_NFILES       50
#define MAX_SIZES        20
#define FIRST_SIZECLASS  3
#define MAX_RECORD_SIZE  33554432
#define MAX_FILESIZE     4294967296

#define RECORD_HEADER_SIZE 8

#define RECORD_NULL 0
#define RECORD_VAL  1
#define RECORD_DEL  2
#define RECODE_END  3

// Record header
// TODO: Avoid duplicates
typedef struct __attribute__((__packed__)) {
    uint8_t     type; // type of record
    uint8_t     extlen; // extra length - key size
    uint16_t    seqno; // future journaling stuff
    uint32_t    len;  // total size of record
} record_header;

// Data record
typedef struct __attribute__((__packed__)) {
    uint8_t     type; // type of record
    uint8_t     extlen; // extra length - key size
    uint16_t    seqno; // future journaling stuff
    uint32_t    len;  // total size of record
    char        data[0]; // data
} record;

// Location
typedef union {
    struct __attribute__((__packed__)) {
        uint16_t num; // File number
        uint16_t sclass; // Size class
        uint32_t offset; // Offset within file
    } l;
    uint64_t val; // Represent as record id
} loc;

// Data struct to keep size list belonging to a class
// TODO: Replace with a rbtree to ensure O(logn) and improve fragmentation
typedef struct _freeloc {
    loc l;
    struct _freeloc *prev, *next;
} freeloc;

freeloc *freeloc_new(loc l);

freeloc *freelist_add(freeloc *head, freeloc *n);

freeloc *freelist_get(freeloc *head, uint32_t size);

freeloc *freelist_remove(freeloc *head, freeloc *f);

typedef struct {
    uint16_t  version; // Lightkv version
    char      *basepath; // Base db directory path
    void      *filemaps[MAX_NFILES]; // Pointer to file mmaps
    uint16_t  nfiles; // Currently initialized max files
    bool      prealloc; // Need pre-file allocation
    loc       start_loc, end_loc; // Location reference to start and current end
    freeloc   *freelist[MAX_SIZES]; // Slab allocation list
    int       error; // err num
    bool      has_scanned;
} lightkv;

// Create a file with given size and fill zeros
int alloc_file(char *filepath, size_t size);

// Memory map an existing file
int map_file(void **map, char *filepath);

// Allocate the next location
loc create_nextloc(lightkv *kv, uint32_t size);

// Write record into disk
int write_record(lightkv *kv, loc l, record *rec);

// Read record from a location
int read_record(lightkv *kv, loc l, record **rec);

// Read record header from a location
record_header read_recheader(lightkv *kv, loc l);

// Create a record
record *create_record(uint8_t type, char *key, char *val, size_t len, size_t recsize);

// Find or create a free loc to store record of given size
loc find_freeloc(lightkv *kv, size_t size);

// Public methods

// Initialize db
int lightkv_init(lightkv **kv, char *base, bool prealloc);

// Has error occured?
bool lightkv_has_error(lightkv *kv);

// Get error string
char *lightkv_errorstr(lightkv *kv);

// Insert
uint64_t lightkv_insert(lightkv *kv, char *key, char *val, uint32_t len);

// Update
uint64_t lightkv_update(lightkv *kv, uint64_t recid, char *key, char *val, uint32_t len);

// Delete
bool lightkv_delete(lightkv *kv, uint64_t recid);

// Get
bool lightkv_get(lightkv *kv, uint64_t recid, char **key, char **val, uint32_t *len);

// Lightkv iterator object
typedef struct {
    lightkv *store;
    loc     current;
} lightkv_iter;

// Scan whole db
lightkv_iter *lightkv_iterator(lightkv *kv);

// Get next item
bool lightkv_next(lightkv_iter *iter, char **key, char **val, uint32_t *len);

// Free iterator
void lightkv_free_iter(lightkv_iter *iter);


#endif
