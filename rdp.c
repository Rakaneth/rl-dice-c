#include "rdp.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define CUR(l) (l->str[l->cursor])
#define IS_DIGIT(c) ((c) >= '0' && (c) <= '9' )
#define DIGIT(c) ((c) - '0')
#define IS_EOF(c) ((c) == '\0')
#define ADVANCE(l) (l->cursor++)

static unsigned int rng_state = 0xDEADBEEF;

void rng_init() {
    rng_state = (unsigned int)time(NULL);
}

static unsigned int xorshift32() {
    rng_state ^= rng_state << 13;
    rng_state ^= rng_state >> 17;
    rng_state ^= rng_state << 5;
    return rng_state;
}

typedef struct lex_t {
    size_t cursor;
    char* str;
} Lex;

static Lex* lex_create(const char* str) {
    Lex* l = (Lex*)malloc(sizeof(Lex));
    l->cursor = 0;
    l->str = strdup(str);
    return l;
}

static void lex_destroy(Lex* l) {
    free(l->str);
    free(l);
}

static void lex_skip_ws(Lex* l) {
    char c = CUR(l);
    while (c == ' ' || c == '\t') {
        ADVANCE(l);
        c = CUR(l);
    }
}

static int lex_num(Lex* l) {
    char c = CUR(l);
    int acc = 0;

    while (IS_DIGIT(c)) {
        acc = acc * 10 + DIGIT(c);
        ADVANCE(l);
        c = CUR(l);
    }

    return acc;
}

static int expr(Lex* l);
static int term(Lex* l);
static int dice(Lex* l, int num);

int next_int(int low, int high) {
    int range = high - low + 1;
    int r = xorshift32() % range;
    return r + low;
}

int die_roll(int num, int sides) {
    int n = (num < 1) ? 1 : num;
    int s = (sides < 1) ? 1 : sides;
    int acc = 0;

    for (int i=0; i<n; i++) {
        acc += next_int(1, s);
    }

    return acc;
}

bool next_bool() {
    return (xorshift32() & 1) == 1;
}

static void print_err(Lex* l, const char* msg) {
    fprintf(stderr, "Parsing error at char %zu (%c): %s", l->cursor, CUR(l), msg);
    exit(1);
}

static int expr(Lex* l) {
    int a = 0;
    char c = CUR(l);

    lex_skip_ws(l);

    a = term(l);
    c = CUR(l);
    while (!(IS_EOF(c))) {
        lex_skip_ws(l);
        c = CUR(l);
        if (c == '+') {
            ADVANCE(l);
            a = a + term(l);            
        } else if (c == '-') {
            ADVANCE(l);
            a = a - term(l);
        }
    }

    return a;
}

static int term(Lex* l) {
    int a = 0;
    char c = 0;

    lex_skip_ws(l);
    c = CUR(l);

    if (c == 'd') {
        ADVANCE(l);
        return dice(l, 1);
    } else if (IS_DIGIT(c)) {
        a = lex_num(l);
        c = CUR(l);
        if (c == 'd') {
            ADVANCE(l);
            return dice(l, a);
        } else {
            return a;
        }
    }

    print_err(l, "malformed term");
    return -1;
}

static int dice(Lex* l, int num) {
    char c = CUR(l);
    int b = 0;

    if (!IS_DIGIT(c)) {
        print_err(l, "expected digit");
    }

    b = lex_num(l);
    return die_roll(num, b);
}

int roll(const char* str) {
    Lex* lex = lex_create(str);
    int result = expr(lex);
    lex_destroy(lex);
    return result;
}

int gauss_int(int median) {
    int acc = 0;
    int sides = median * 2;
    for (int i=0; i<3; i++) {
        acc += die_roll(1, sides);
    }
    return acc / 3;
}





