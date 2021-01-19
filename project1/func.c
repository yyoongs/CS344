#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func.h"

typedef enum {false,true} bool;
// define bool to use boolean type

void func1(struct movie *list) // function 1
{
    int year; // input year
    int count = 0; // to use when I check how many movies are printed
    printf("Enter the year for which you want to see movies: ");
    scanf("%d",&year);
    while (list!= NULL) // repeat linked list
    {
        if(year == get_movie_year(list)) // if input year is same as linked list's year
        {
            print_movie_title(list); // print movie title
            count ++; // check printed movie count
        }
        list = list->next; // change to next node
    }
    if(count == 0) // if input year doesn't match anything
    {
        printf("No data about movies released in the year %d",year);
    } // print notification message
    printf("\n\n");
}


void func2(struct movie *list, int movie_count) // function 2
{
    struct movie *head = list; // struct head is first movie in linked list movies
    struct movie *curr = head; // struct curr is head which is first movie in linked list movies
    int* count_year = calloc(movie_count,sizeof(int)); // allocate memory 
    int count = 0;
    bool check = false;

    while (list!= NULL)
    {
        curr = head; // curr locate first of linked list movies
        while(curr!=NULL) // repeat linked list
        {
            if(get_movie_year(curr) == get_movie_year(list) && get_movie_rate(curr) > get_movie_rate(list))
            {
                break;
                // if list's year is same as current year and list's rate is less than currnet rate, break while roof
                // because the movie that has lower rate than other movie shouldn't be printed 
            }
            if(strcmp(curr->title,list->title)!=0 && get_movie_year(curr) == get_movie_year(list) && get_movie_rate(curr) == get_movie_rate(list))
            {
                // if 2 different movies have same year and same rate 
                for(int c = 0;count_year[c]!=0;c++)
                {
                    if(curr->year == count_year[c]) // if current movie's year is in the count_year list
                    {
                        check = true; // change boolean true and do not print
                        break;
                    }
                }
                if(check) // do not print same year and same rate movie
                {
                    check = false;
                    break;
                }
                else // print movie that has highest rate in each year and add to count_year list
                {
                    count_year[count] = curr->year;
                    count++;
                    printmovie_func2(list);
                    break;
                }

            }
            if(curr->next == NULL) // print movie that has highest rate in each year and add to count_year list
            {
                printmovie_func2(list);
                break;
            }
            else 
            {
                curr = curr->next;
            }
        }
        list = list->next; // search next linked list
    }
    printf("\n");
    free(count_year); // memory free
}


void func3(struct movie *list) // function 3
{
    printf("Enter the language for which you want to see movies: ");

    char* input_lan = malloc(20*sizeof(char)); // allocate input language memory by using malloc
    scanf("%s",input_lan); // input language
    int check = 0; // to use when I check how many movies are printed
    
    while(list!=NULL) // repeat linked list
    {
        for(int count = 0;count<5;count++) // check languages in linked list
        {
            if(strcmp(list->languages[count],input_lan)==0) // compare languages using strcmp
            {
            printmovie_func3(list); // print movie year and title
            check++; // count movie number
            break;
            }
        }
        list = list->next; //move to next node
    }

    if(check==0) // if printed movie number is 0
    {
        printf("No data about movies released in %s\n",input_lan);
    } // print notification message
    printf("\n");
    free(input_lan); // memory free
}
