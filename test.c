#include "lightkv.h"
#include "logger.h"
#include <stdio.h>


int main() {
    lightkv *kv;
    lightkv_init(&kv,(char *)  "/tmp/", true);
    uint64_t rid;
    char *k,*v;
    uint32_t l;
    int i;
    uint64_t recid;
    lightkv_iter *it = lightkv_iterator(kv);
    while (lightkv_next(it, &recid, &k, &v, &l)) {
        free(k);
        free(v);
    }

    rid = lightkv_insert(kv, "test_key1", "hello", 5);

    rid = lightkv_insert(kv, "test_key2", "helli", 5);
    lightkv_delete(kv, rid);



    lightkv_get(kv, rid, &k, &v, &l);

    rid = lightkv_update(kv, rid, "test_upd", "updat", 5);
    rid = lightkv_update(kv, rid, "test_update-large", "1234567890", 10);


    for (i=0; i < 1 << 25; i++) {
        char *st = (char *) calloc(10,1);
        sprintf(st, "key_%d", i);

        rid = lightkv_insert(kv, st, (char *) "hell3", 5);
        free(st);
    }
    lightkv_free_iter(it);



    it = lightkv_iterator(kv);

    i = 0;
    while (lightkv_next(it, &recid, &k, &v, &l)) {
        free(k);
        free(v);
        i++;
    }
    lightkv_free_iter(it);
    lightkv_close(kv);
    debug_log("read %d items", i);
    exit(0);

}
