#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include "func.h"

#define _CRT_SECURE_NO_WARNINGS
#define NAME_MAX 255

typedef enum {false,true} bool;
// define bool to use boolean type


void print_sub_menu() // print sub menu
{
    printf("Which file you want to process?\n");
    printf("Enter 1 to pick the largest file\n");
    printf("Enter 2 to pick the smallest file\n");
    printf("Enter 3 to specify the name of a file\n\n");
    printf("Enter a choice from 1 to 3: ");
}

void make_dir(char* file_name) // parameter : file_name, function of making directory that name is file_name
{
    srand((unsigned int)time(NULL)); // set srand by using time funciton (define in time.h)
    int number = rand(); // set random number
    number = number % 100000; // set range 0-99999
    // refer to https://blockdmask.tistory.com/308

    char str_num[5];
    char* dir_name = malloc(21*sizeof(char));
    strcpy(dir_name,"choyongs.movies.");
    sprintf(str_num,"%d",number);
    strcat(dir_name,str_num); // set directory name as choyongs.movies.NUMBER

    mkdir(dir_name,0750); // make directory and set mode
    char* change_dir = malloc(23*sizeof(char));
    strcpy(change_dir,"./");
    strcat(change_dir,dir_name); // set path in order to move made directory 

    struct movie *list = processFile(file_name); // read csv file

    int change_result = chdir(change_dir); // change working directory as made directory
    // refer to https://shaeod.tistory.com/325

    if(change_result == -1) // if it can't change directory, print error message
    {
        printf("fail moving\n");
        exit(-1);        
    }

    char str_year[9];

    while(list!=NULL) // check linked list
    {
        int year = list->year;
        sprintf(str_year,"%d",year);
        strcat(str_year,".txt"); // make file name YYYY.txt

        FILE *fp = fopen(str_year, "a"); // file open (append)
        fputs(list->title,fp); // write movie title
        fputs("\n",fp);
        fclose(fp); // file close
        int change_mod = chmod(str_year,0640); // change file mode
        // refer to https://badayak.com/4413

        if(change_mod == -1) // if error, print message
        {
            printf("fail\n");
            exit(-1);
        }
        list = list->next;
    }
    printf("Created directory with name %s\n\n",dir_name);
    
    change_result = chdir(".."); // change working directory to first directory
    if(change_result == -1)
    {
        printf("fail moving\n");
    }
    
    // memory free
    free(dir_name); 
    free(change_dir);

}


void sub_func1()
{
    DIR *dp;
    struct dirent *dir;
    struct stat buf;
    int cmp = 0;
    char* fname;

    if((dp=opendir("."))==NULL) // try open current working directory
    {
        fprintf(stderr,"directory open error\n");
        exit(-1);
    }

    while((dir=readdir(dp))!=NULL) // read directory
    {
        if(strcmp(dir->d_name,"movies_by_year")==0) // if file name is movies_by_year, pass checking
        {
            continue;
        }

        int size = 0;
        lstat(dir->d_name,&buf); // check file stat
        if( S_ISREG(buf.st_mode)) // if file is regular file (not directory)
        {
            char* ptr = strrchr(dir->d_name,'.');
            // refer to https://dojang.io/mod/page/view.php?id=370

            if(strncmp(dir->d_name,"movies_",7)==0 && strcmp(ptr,".csv")==0) // startwith movies_ && endwith .csv
            {
                FILE *fp = fopen(dir->d_name,"r");
                fseek(fp,0,SEEK_END);
                size = ftell(fp);
                // check file size

                // save the biggest file information
                if(cmp == 0)
                {
                    cmp = size;
                    fname = dir->d_name;
                }
                else if(size>cmp)
                {
                    cmp = size;
                    fname = dir->d_name;
                }

                fclose(fp); // file close
            }
        }
    }
    printf("Now processing the chosen file named %s\n",fname);

    make_dir(fname);
    closedir(dp);
}

void sub_func2()
{
    DIR *dp;
    struct dirent *dir;
    struct stat buf;
    int cmp = 0;
    char* fname;

    if((dp=opendir("."))==NULL) // try open current working directory
    {
        fprintf(stderr,"directory open error\n");
        exit(-1);
    }

    while((dir=readdir(dp))!=NULL) // read directory
    {
        if(strcmp(dir->d_name,"movies_by_year")==0) // if file name is movies_by_year, pass checking
        {
            continue;
        }
        
        int size = 0;
        lstat(dir->d_name,&buf); // check file stat

        if( S_ISREG(buf.st_mode)) // if this file is regular file (not directory)
        {
            char* ptr = strrchr(dir->d_name,'.');

            if(strncmp(dir->d_name,"movies_",7)==0 && strcmp(ptr,".csv")==0) // startwith movies_ && endwith .csv
            {    
                FILE *fp = fopen(dir->d_name,"r");
                fseek(fp,0,SEEK_END);
                size = ftell(fp);
                // check file size

                // save the biggest file information
                if(cmp == 0)
                {
                    cmp = size;
                    fname = dir->d_name;
                }
                else if(size<cmp)
                {
                    cmp = size;
                    fname = dir->d_name;
                }
                fclose(fp); // file close
            }
        }
    }
    printf("Now processing the chosen file named %s\n",fname);
    // print notification sentence and make directory with file name
    make_dir(fname);
    closedir(dp);
}

void func1() // sub_main function
{
    while(1)
    {
        int input_num;
        print_sub_menu();
        scanf("%d",&input_num);
        if(input_num == 1) // number 1
        {
            sub_func1();
            break;
        }
        if(input_num == 2) // number 2
        {
            sub_func2();
            break;
        }
        if(input_num == 3) // number 3
        {
            DIR *dp;
            struct dirent *dir;
            struct stat buf;
            char* input_name = malloc(NAME_MAX * sizeof(char)); // input file name
            bool check = false; // to check processed file

            printf("Enter the complete file name: ");
            scanf("%s",input_name); // input file name that you want to search

            if((dp=opendir("."))==NULL) // try open current working directory
            {
                fprintf(stderr,"directory open error\n");
                exit(-1);
            }

            while(1)
            {
                check = false; // to check processed file
                while((dir=readdir(dp))!=NULL) // read directory
                {
                    lstat(dir->d_name,&buf); // check current file stat

                    if( S_ISREG(buf.st_mode)) // if this file is regular file (not directory)
                    {
                        if(strcmp(input_name,dir->d_name) == 0) // if input file name is same as current opened file
                        {
                            char* ptr = strrchr(dir->d_name,'.');
                            if(ptr == NULL)
                            {
                                printf("file's extension is not .csv\n");
                                continue;
                            }
                            if(strcmp(ptr,".csv")==0) // if current opened file endwith .csv
                            {
                                check = true; // change status
                                printf("Now processing the chosen file named %s\n",dir->d_name);
                                make_dir(dir->d_name); // make directory
                                break;
                            }
                            else // file name doesn't endwith .csv
                            {
                                check = true;
                                printf("file's extension is not .csv\n\n");
                                break;
                            }
                        }
                    }
                }
                // if making file directory works succesfully or input file name that doesn't endwith .csv 
                if(check) 
                {
                    break; // end this roop
                }
                else // go back to sub menu
                {
                    printf("The file %s was not found. Try again\n\n",input_name);
                    break;
                }
            }
            // if making file directory works succesfully or input file name that doesn't endwith .csv 
            // go back to main menu
            if(check)
            {
                break;
            }
            free(input_name); // memory free
            closedir(dp); // close directory
        }
        else
        {
            printf("default try again!\n");
            break;
        }
    }
    
}
