#define GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func.h"

int movie_count = 0;


void print_menu() // print menus
{
    printf("1. Select file to process\n");
    printf("2. Exit the program\n");
    printf("Enter a choice 1 or 2: ");
}

// this is main function
int main(int argc, char *argv[])
{
    int input;

    while(1)
    {
        print_menu();
        scanf("%d", &input); //read menu number
        if(input == 2){ // if input number is 2, end program
            break;
        }
        else{
            switch(input){
            case 1 :
                // if input number is 1
                func1();
                break;
            default :
                // error message
                printf("You entered an incorrect choice. Try again.\n\n");
            }
        };
    };

    return EXIT_SUCCESS;
}
