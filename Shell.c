#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAX_ARGS 15
#define MAX_LINE 200

// handling EOF
void remove_end_line(char line[]){
    int i =0;
    while(line[i] != '\n')
    {
        i++;
    }
    printf("%d", i);
    line[i] = '\0';
}

// reading shell command line
void read_line(char line[]){
    fgets(line,MAX_LINE,stdin);
    remove_end_line(line);
    printf("\n I have read the following line: \n%s\n", line);

}

// parsing line
int split_line(char line[],char* args[]){
    int i = 0;
    args[i] = strtok(line," ");

    if(args[i] == NULL)
    {
        printf("NO COMMANS !\n");
        return 1;
    }
    while(args[i] != NULL)
    {
        i++;
        args[i] = strtok(NULL," ");

    }
    return 1;
}


int main()
{
    // variables declaration
    char* args[MAX_ARGS];
    char line[MAX_LINE];
    while(1){
	    read_line(line);    // calling reading command line function
	    split_line(line,args);  // parsing the line
    }
    // execute the parsing line
    pid_t child_pid = fork();
    if(child_pid == 0){
        execvp(args[0],args);
    }
    else{
        waitpid(child_pid,NULL,0);
    }

    return 0;
}
