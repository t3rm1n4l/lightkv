#ifndef HELPER_H
#define HELPER 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATAFILE_FORMATSTR  "data.%d.db"

char *joinpath(char *base, char *next) {
    int l1,l2;
    char *s;
    l1 = strlen(base);
    l2 = strlen(next);

    if (base[l1-1] == '/') {
        l1--;
    }

    s = malloc(l1+l2+2);
    strncpy(s, base, l1);
    strncpy(s+l1+1, next, l2);
    s[l1] = '/';
    s[l1+l2+2] = '\0';

    return s;
}

char *getfilepath(char *base, int n) {
    char name[200];
    // TODO: Fail checks
    snprintf(name, 200, DATAFILE_FORMATSTR, n);
    return joinpath(base, name);
}

#endif
