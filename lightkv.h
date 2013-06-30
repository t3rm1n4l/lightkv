#ifndef LIGHTKV_H
#define LIGHTKV_H 1

#define MAX_NFILES    50
#define MAX_SIZES     20
#define FIRST_SIZE    (1 << 3)
#define MAX_RECORD_SIZE (1 << 5)

// Data record
typedef struct {
    uint8_t    type;
    uint8_t    len;
    uint8_t    seqno;
    uint8_t    extlen;
    char       []data;
} record;

// Location
typedef union {
    struct {
        uint16_t num; // File number
        uint32_t offset; // Offset within file
        uint16_t sclass; // Size class
    } l;
    uint64_t val; // Represent as record id
} loc;

// Data struct to keep size list belonging to a class
// TODO: Replace with a rbtree to ensure O(logn) and improve fragmentation
typedef struct _freeloc {
    uint32_t size;
    _freeloc *next;
} freeloc;

// Helper methods to operate on free list
static freeloc *freeloc_new(record *r);

static freeloc *freelist_add(freeloc *head, freeloc *n);

static freeloc *freelist_get(freeloc *head, uint32_t size);

typedef struct {
    uint16_t  version; // Lightkv version
    char      *basepath; // Base db directory path
    void      *filemaps[MAX_NFILES]; // Pointer to file mmaps
    uint16_t  nfile; // Currently initialized max files
    bool      prealloc; // Need pre-file allocation
    loc       start_loc, end_loc; // Location reference to start and current end
    freeloc   *freelist[MAX_SIZES]; // Slab allocation list
    int       error; // err num
    bool      has_scanned;
} lightkv;

// Create a new lightdb
static lightkv *lightkv_new(char *base, bool prealloc);

// Open an existing db
// TODO: implement later
static lightkv *lightkv_open(char *base, bool prealloc);


// Public methods

// Initialize db
lightkv *lightkv_init(char *base, bool prealloc);

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
    loc     *current;
} lightkv_iter;

// Scan whole db
lightkv_iter *lightkv_iterator(lightkv *kv);

// Get next item
bool lightkv_next(lightkv_iter *iter, char **key, char **val, uint32_t *len);

// Free iterator
void lightkv_free_iter(lightkv_iter *iter);


#endif
