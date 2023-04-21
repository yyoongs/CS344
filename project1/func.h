#define GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "movie.h"

typedef enum {false,true} bool;

void func1(struct movie *list);
void func2(struct movie *list, int movie_count);
void func3(struct movie *list);