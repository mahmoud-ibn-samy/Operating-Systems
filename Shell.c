#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#define MAX_ARGS 15
#define MAX_LINE 200



void remove_end_line(char line[]){
    int i =0;
    while(line[i] != '\n'&&line[i]!='>')
    {
        i++;
    }
    line[i] = '\0';
}

int detect_redirection(char line[], char file_name[]){
    int i = 0;
    int ret;
    while (line[i]!=NULL&&line[i]!='>'&&line[i]!='<'){
        i++;
    }
    if (line [i]==NULL)
        return -1;
    if (line[i]=='>')
        ret=1;
    else
        ret=0;
    i++;
    int j=0;
    if (line [i]==' ')
        i++;
    while (line[i]!=NULL){
        file_name[j]=line[i];
        i++; j++;
    }

    file_name[j-1]=NULL;

    return ret;
}
// reading shell command line
int read_line(char line[],char file_name[]){

    fgets(line,MAX_LINE,stdin);
    int ret=detect_redirection(line,file_name);
    remove_end_line(line);
    return ret;
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

    char line[20], file_name[25];
    char * args[20];
    while(1){
        printf("sish:> ");
        int ret= read_line(line,file_name);
        split_line(line,args);  // parsing the line

        // execute the parsing line
        pid_t child_pid = fork();
        if(child_pid == 0){
            if (ret!=-1){
                int file = open(file_name, O_RDWR|O_CREAT,0777);
                dup2(file,ret);
            }

            int a = execvp(args[0],args);
            if(a == -1){
                 printf("Please Enter A Valid Shell Command\n");
            }
        }
        else{
            waitpid(child_pid,NULL,0);
        }

    }


    return 0;
}
