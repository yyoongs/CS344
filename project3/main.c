#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <termios.h>

#define MAX_ARGUMENTS 512
#define MAX_CHARACTERS 2048

struct COMMAND{ // command structure
        char* name;
        char* desc;
        bool ( *func )( int argc, char* argv[] ); // function pointer
};


// global
pid_t child=-1; // child pid 
int status = 0; // process status
int pid;
int fd,fd2;
bool background = true; // for SIGT

// **************** function ******************
static void handler(int signo);
void append(char *destination, char c);
bool cmd_cd( int argc, char* argv[]);
bool cmd_exit( int argc, char* argv[]);
bool cmd_status( int argc, char* argv[]);
bool redirect( int argc, char* argv[] );
int tokenize( char* buf, char* tokens[], int maxTokens);
bool run_func(char *buf);

struct COMMAND  builtin_cmds[] =
{
    { "cd",    "change directory", cmd_cd  },
    { "exit",   "exit this shell", cmd_exit  },
    { "status",   "status", cmd_status  }
}; // built in command

static void handler(int signo) // SIGTSTP handler
{
    if(background == false)
    {
        background = true;
        printf("\nExiting foreground-only mode\n");
        fflush(stdout);
    }
    else
    {
        background = false;
        printf("\nEntering foreground-only mode (& is now ignored)\n");
        fflush(stdout);
    }

}

void append(char *destination, char c) {
    char *p = destination;
    while (*p != '\0') p++; // find end of string
    *p = c;
    *(p+1) = '\0'; 
}


bool cmd_cd( int argc, char* argv[] )
{ //cd : change directory
        if( argc == 1 )
        {
            // printf("home\n");
            chdir( getenv( "HOME" ) );
        }
                
        else if( argc == 2 ){
            if( chdir( argv[1] ) )
                printf( "No directory\n" );
                fflush(stdout);

        }
        else
            printf( "USAGE: cd [dir]\n" );
            fflush(stdout);

    return true;
}

bool cmd_exit( int argc, char* argv[] )
{
    exit(0);

    return true;
}

bool cmd_status( int argc, char* argv[] )
{
        printf("exit value %d\n",status);
        fflush(stdout);
    return true;
}

bool redirect( int argc, char* argv[] )
{
    char *p[] = { argv[0], NULL};
    char buf[MAX_CHARACTERS];

    child = fork();
    if( child == 0 ){
        if(argc==5) // if '<' and '>' run same line
        {
            fd= open(argv[4],O_WRONLY|O_CREAT|O_TRUNC,770); // open "junk2"

            if(fd == -1)
            {
                perror("failed open fd1");      return 1;
            }
            if(dup2(fd,STDOUT_FILENO) == -1) // dup2 => fd duplicate to stdout
            {
                perror("failed dup2");      return 1;
            }
            close(fd);

            fd2 = open(argv[2],O_RDONLY);  // open "junk"
            if(fd2 == -1)
            {
                perror("failed open fd2");      return 1;
            }
            if(dup2(fd2,STDIN_FILENO) == -1)
            {
                perror("failed dup2");      return 1;
            }
            close(fd2);

            execvp( argv[0], p ); // exec "wc"
            // printf( "No such file\n" );
            perror(argv[0]);
        }

        else if(strcmp(argv[1],">")==0) // if only '>' command 
        {
            fd= open(argv[2],O_WRONLY|O_CREAT|O_TRUNC, 770);
            if(fd == -1)
            {
                perror("failed open ");      return 1;
            }
            if(dup2(fd,STDOUT_FILENO) == -1) // fd duplicate to stdout
            {
                perror("failed dup2");      return 1;
            }
            close(fd);
            execvp( argv[0], p );
            perror(argv[0]);
        }

        else if(strcmp(argv[1],"<")==0) // if only '<' command
        {
            fd= open(argv[2],O_RDONLY);
            if(fd == -1)
            {
                perror(argv[2]);      return 1;
            }
            if(dup2(fd,STDIN_FILENO) == -1) // fd duplicate to stdin
            {
                perror("failed dup2");      return 1;
            }
            close(fd);
            execvp( argv[0], p );
            perror(argv[0]);
        }
        
    }
    else if( child < 0 ) // if fork fail
    {
        printf( "Failed to fork()!" );
        fflush(stdout);
        _exit( 0 );
    }
    else // foreground
    { 
        child = waitpid(child, &status, 0 );
        if(WIFEXITED(status)) // check status and save
        {
            status = WEXITSTATUS(status);
        } 
        else
        {
            status = WTERMSIG(status);
        }
    }

    return true;
}

int tokenize( char* buf, char* tokens[], int maxTokens)
{
        int token_count = 0;
        char* token;
        char *saveptr; //to use strtok_r

        token = strtok_r( buf, " \n" , &saveptr);
        while( token != NULL && token_count < maxTokens ){ // make token by using strtok_r
                tokens[token_count] = token;
                token_count++;
                token = strtok_r( NULL, " \n", &saveptr);
        }
        tokens[token_count] = NULL;
        return token_count;
}

bool run_func(char *buf)
{
        char* tokens[MAX_ARGUMENTS];
        int token_count;
        int i;
        bool back = false;
        char s1[10]; // to save process number
        bool redi = false;
        
        sprintf(s1, "%d", getpid()); // int to char process id

        char* changed = malloc(MAX_CHARACTERS*sizeof(char));
        for(i=0;i<strlen(buf);i++)
        {
                if(buf[i] == '$' && buf[i+1] == '$')
                {
                    strcat(changed,s1);
                    i++;
                }
                else
                {
                    append(changed,buf[i]);
                }
        } // check $$ word
        // if $$ is detected, changed to process id

        for(i=0;i<strlen(changed);i++){ // check background
                if(changed[i] == '&'){
                        back=true; //do not wait background process
                        changed[i]='\0';
                        break;
                }
        }

        for(i=0;i<strlen(changed);i++)
        { // redirection check
            if(changed[i] == '>' || changed[i] == '<')
            {
                    redi=true;
                    break;
            }
        }

        token_count = tokenize( changed, tokens, sizeof( tokens ) / sizeof( char* ) ); // make command line token

        // ************************* empty input *************************
        if( token_count == 0 ) // input = empty
        {
            while ( (pid = waitpid(-1, &status, WNOHANG)) > 0) // check finished background procees 
            {
                if(WIFEXITED(status))
                {
                    printf("background pid %d is done: exit value %d\n", pid, WEXITSTATUS(status));
                    fflush(stdout);
                } 
                else
                {
                    printf("background pid %d is done: terminated by signal %d\n", pid, WTERMSIG(status));
                    fflush(stdout);

                }
            }
            return true;
        }


        // ********************** built in command ******************************
        for( i = 0; i < sizeof( builtin_cmds ) / sizeof( struct COMMAND ); i++ ){
                if( strcmp( builtin_cmds[i].name, tokens[0] ) == 0 )
                {
                    return builtin_cmds[i].func(token_count, tokens);
                }
                        
        }


        // ********************** ctrl + Z check ******************************
        if(background == false && back == true)
        {
            return true;    // background ignore
        }
    
        // ********************** comment line ******************************
        if(strcmp(tokens[0],"#")==0) // comment line
        {
            return true;
        }

        if(redi == true) // redirection check
        {
            if(strcmp(tokens[1],"<")==0 || strcmp(tokens[1],">")==0)
            {
                redirect(token_count, tokens);
            }
        }
        else
        {
            child = fork();
            if( child == 0 ){
                signal(SIGINT, SIG_DFL); // SIGINT activate
                execvp( tokens[0], tokens ); // exec command line
                perror(tokens[0]);
            }
            else if( child < 0 ) // if fork fail
            {
                printf( "Failed to fork()!" );
                _exit( 0 );
            }
            else if(back == false) // foreground process
            { 
                child = waitpid(child, &status, 0 ); // wait child process
                if(WIFEXITED(status)) // exit value check
                {
                    // printf("Child %d exited normally with status %d\n", child, WEXITSTATUS(status));
                    status = WEXITSTATUS(status);
                }
                else if(WIFSIGNALED(status))
                {
                    printf("terminated by signal %d\n",WTERMSIG(status));
                    fflush(stdout);
                    status = WTERMSIG(status);
                } 
                else
                {
                    // printf("Child %d exited abnormally due to signal %d\n", child, WTERMSIG(status));
                    status = WTERMSIG(status);
                }
            }
            else if(back == true) // background process
            {   
                printf("background pid is %d\n",child);
                fflush(stdout);
                child = waitpid(child, &status, WNOHANG ); // do not wait child process
            }
        }

        // ********************* back ground check ***********************
        while ( (pid = waitpid(-1, &status, WNOHANG)) > 0)
        {
            if(WIFEXITED(status))
            {
                printf("background pid %d is done: exit value %d\n", pid, WEXITSTATUS(status));
                fflush(stdout);
            } 
            else
            {
                printf("background pid %d is done: terminated by signal %d\n", pid, WTERMSIG(status));
                fflush(stdout);
            }
        }
        // ********************* back ground check ***********************

        return true;
}

int main()
{
    // signal ignore
    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);

    // ***********************signal*******************
    struct sigaction SIGTSTP_action = {0}; // SIGTSTP_action struct
    // Register handle_SIGINT as the signal handler
    SIGTSTP_action.sa_handler = handler; // Register handle_SIGTSTP as the signal handler
    sigfillset(&SIGTSTP_action.sa_mask); // Block all catchable signals
    SIGTSTP_action.sa_flags = 0; // No flags set
    sigaction(SIGTSTP, &SIGTSTP_action, NULL);
    signal(SIGTSTP,handler); // using our handler
    // ***********************signal*******************
            
    while (1)
    {
        fflush(stdin);
        tcflush(0, TCIFLUSH);

        fflush(stdout);
        printf(": ");
        char * buf = malloc(MAX_CHARACTERS*sizeof(char)); // command lines with a maximum length of 2048
        size_t len = 0;
        ssize_t line_size = 0;
        line_size = getline(&buf,&len,stdin); // read command lines
        fflush(stdout);

        run_func(buf); // run my shell
    }
    return 0;
}