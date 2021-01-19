#define GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func.h"

int movie_count = 0;


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
        movie_count++; // count the number of movie
    }
    free(currLine); // memory free
    fclose(movieFile);
    printf("Processed file %s and parsed data for %d movies\n\n", filePath, movie_count);
    return head;
}

void print_menu() // printing menus
{
    printf("1. Show movies released in the specified year\n");
    printf("2. Show highest rated movie for each year\n");
    printf("3. Show the title and year of release of all movies in a specific language\n");
    printf("4. Exit from the program\n\n");
    printf("Enter a choice from 1 to 4: ");
}

// this is main function
int main(int argc, char *argv[])
{
    if (argc < 2) 
    {
        printf("You must provide the name of the file to process\n");
        printf("Example usage: ./movies movies_sample_1.csv\n");
        return EXIT_FAILURE;
    }
    int input;
    struct movie *list = processFile(argv[1]);

    while(1)
    {
        print_menu();
        scanf("%d", &input); //read menu number
        if(input == 4){
            break;
        }
        else{
            switch(input){
            case 1 :
                // if input number is 1
                func1(list);
                break;
            case 2 :
                // if input number is 1
                func2(list,movie_count);
                break;
            case 3 :
                // if input number is 1
                func3(list);
                break;
            default :
                // error message
                printf("You entered an incorrect choice. Try again.\n\n");
            }
        };
    };

    return EXIT_SUCCESS;
}
