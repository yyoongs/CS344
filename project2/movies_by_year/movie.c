#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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


void RemoveLast(char *buf) // remove last word
{
  int len = strlen(buf);
  buf[len-1] = '\0';
}

void RemoveFirst(char *buf) // remove first word
{
    int i = 0;
    for (i = 1; buf[i]; i++)//to repeat if buf[i] is true
    {
        buf[i - 1] = buf[i]; // buf[i] to buf[i-1]
    }
    buf[i - 1] = '\0';
}

struct movie *createmovie(char *currLine)
{
    struct movie *currmovie = malloc(sizeof(struct movie));
    char *saveptr; //to use strtok_r
    char *saveptr2; //to use strtok_r
    char *curr; //to use strtok_r
    char *remain; //to use strtok_r

    char *token = strtok_r(currLine, ",", &saveptr); // make token about movie title
    currmovie->title = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currmovie->title, token); // copy token to title

    token = strtok_r(NULL, ",", &saveptr); // make token about released year
    currmovie->year = atoi(token); // change string to integer and move to year
    
    token = strtok_r(NULL, "]", &saveptr); // next token
    remain = strtok_r(NULL, "\n", &saveptr); // save next token

    char *token2 = strtok_r(token,"[", &saveptr2); // remove '['
    int count = 0; // to count languages for each movie
    curr = strtok_r(token2, ";", &saveptr2); // make token abut languages
    strcpy(currmovie->languages[count],curr); // copy to linked list structure

    while(1) // repeat make language token 
    {
        curr = strtok_r(NULL,";",&saveptr2); // make token abut languages
        if(curr!=NULL)
        {
            ++count;
            strcpy(currmovie->languages[count],curr); // save token
        }
        else
        {
            break;
        }
    }
    RemoveFirst(remain); // remove first letter ',' 
    currmovie->rating = strtod(remain, &saveptr); // change string to double 
    // Set the next node to NULL in the newly created movie entry
    currmovie->next = NULL;

    return currmovie;
}

struct movie *processFile(char *filePath)
{
    // Open the specified file for reading only
    FILE *movieFile = fopen(filePath, "r");

    char *currLine = malloc(sizeof(char));
    size_t len = 0;
    ssize_t nread;

    nread = getline(&currLine, &len, movieFile);

    // The head of the linked list
    struct movie *head = NULL;

    // The tail of the linked list
    struct movie *tail = NULL;

    // Read the file line by line
    while ((nread = getline(&currLine, &len, movieFile)) != -1)
    {
        // Get a new movie node corresponding to the current line
        struct movie *newNode = createmovie(currLine);

        // Is this the first node in the linked list?
        if (head == NULL)
        {
            // This is the first node in the linked link
            // Set the head and the tail to this node
            head = newNode;
            tail = newNode;
        }
        else
        {
            // This is not the first node.
            // Add this node to the list and advance the tail
            tail->next = newNode;
            tail = newNode;
        }
        // movie_count++; // count the number of movie
    }
    free(currLine); // memory free
    fclose(movieFile);
    // printf("Processed file %s\n\n", filePath);
    return head;
}