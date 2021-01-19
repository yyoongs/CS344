#ifndef __MOVIE_H__
#define __MOVIE_H__

#include <stdio.h>

struct movie
{
    char *title;
    int year;
    char languages[5][20];
    float rating;
    struct movie *next;
}; // movie struct define

int get_movie_year(struct movie* amovie);
float get_movie_rate(struct movie* amovie);
void print_movie_title(struct movie* amovie);
char* get_movie_title(struct movie* amovie);
void printmovie_func2(struct movie* amovie);
void printmovie_func3(struct movie* amovie);

#endif