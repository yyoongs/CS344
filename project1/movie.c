#include <stdio.h>
#include "movie.h"

int get_movie_year(struct movie* amovie){
    return amovie->year;
}
// get movie year from movie

float get_movie_rate(struct movie* amovie){
    return amovie->rating;
}
// get movie rate from movie

void print_movie_title(struct movie* amovie){
    printf("%s\n",amovie->title);
}
// print movie title from movie

char* get_movie_title(struct movie* amovie){
    return amovie->title;
}
// get movie title from movie


void printmovie_func2(struct movie* amovie)
{
    printf("%d %.1f %s\n", amovie->year, amovie->rating, amovie->title);
}
// use this function when I print function2 result

void printmovie_func3(struct movie* amovie)
{
    printf("%d %s\n", amovie->year, amovie->title);
}
// use this function when I print function3 result
