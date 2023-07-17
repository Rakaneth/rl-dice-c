#pragma once

#include <stdbool.h>

void rng_init();
int die_roll(int num, int sides);
int next_int(int low, int high);
bool next_bool();
int roll(const char* str);
int gauss_int(int median);

