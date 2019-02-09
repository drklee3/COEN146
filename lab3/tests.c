#include "tfv2.h"
#include <stdio.h>
#include <assert.h>

int check_substr(char* orig, char* expected, int start, int len) {
    char* out = malloc(sizeof(*out) * ( len + 1));
    substr(out, orig, start, len);
    printf("orig: %s, substr: %s, start: %d, len: %d, cmp: %d\n",
        orig, out, start, len,
        strcmp(out, expected));
    return strcmp(out, expected) == 0;
}

int main() {    
    assert(check_substr("abcdefg", "abcdefg", 0, 7));
    assert(check_substr("abcdefg", "abcdef",  0, 6));
    assert(check_substr("abcdefg", "abcde",   0, 5));
    assert(check_substr("abcdefg", "abcd",    0, 4));
    assert(check_substr("abcdefg", "bcde",    1, 4));
    assert(check_substr("abcdefg", "cdef",    2, 4));

    return 0;
}