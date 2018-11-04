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
int background;
int bgp=0,done=0;


void remove_end_line(char line[]){
    int i =0;
    while(line[i] != '\n' && line[i] != '>')
        i++;

    line[i] = '\0';
}

int detect_redirection(char line[], char file_name[]){
    int i = 0;
    int ret;
    while (line[i] != NULL && line[i] != '>' && line[i] != '<')
        i++;

    if (line [i] == NULL)
        return -1;
    if (line[i] == '>'){
        if(line[i+1] == '>'){
            i++;
            ret = 2;
        }
        else
            ret = 1;
    }
    else
        ret = 0;
    i++;
    int j = 0;
    while (line [i] == ' ')
        i++;
    while (line[i] != NULL){
        file_name[j] = line[i];
        i++;
        j++;
    }

    file_name[j-1] = NULL;

    return ret;
}
// reading shell command line
int read_line(char line[], char file_name[]){
    int i=0;
    char* command=fgets(line,MAX_LINE,stdin);
    int ret = detect_redirection(line,file_name);
    while(line[i]!='\n')
    i++;
    if(line[i-1]=='&'){
	line[i-1]='\n';
        background=1;
    }
    remove_end_line(line);
    if (strcmp(line,"exit")==0 || command==NULL)
    exit(0);
    return ret;
}

// parsing line
int split_line(char line[],char* args[]){
    int i = 0;
    args[i] = strtok(line," ");

    if(args[i] == NULL)
    {
        if (done!=bgp)
		return 1;
        printf("NO COMMANDS !\n");
        return 1;
    }
    while(args[i] != NULL)
    {
        i++;
        args[i] = strtok(NULL," ");

    }
    return 1;
}


//Converting non-alphanumeric characters to spaces
int checkForAlphanumericChars(char line[], char alphanumericChars[]) {

    int i = 0; //outer loop counter
    int isALPHANUMERIC = 0;

    while (line[i] != '\0') {

        int j = 0;  //inner loop counter

        while (alphanumericChars[j] != '\0') {

            if (line[i] == alphanumericChars[j]) {
                isALPHANUMERIC = 1;
                break;
            }

            j++;

        }

        // if it's not alphanumeric, put a space in its place
        if (isALPHANUMERIC == 0){
           
            line[i] = ' ';
        } else {
            isALPHANUMERIC = 0;
        }

        i++;

    }
    return 1;


}


int main()
{

    char line[20], file_name[25];
    char * args[20];

// variable that is used to check for non-alphanumaric
    char alphanumericChars[] = "1234567890QWERTYUIOPLKJHGFDSAZXCVBNMqwertyuioplkjhgfdsazxcvbnm -'_<>|&";
	
    while(1){
        printf("sish:> ");
        int ret= read_line(line,file_name);
	checkForAlphanumericChars(line, alphanumericChars); //Converting non-alphanumeric characters to spaces
        split_line(line,args);  // parsing the line

        // execute the parsing line
        pid_t child_pid = fork();
        if(child_pid >0){
            if(background==1){
                bgp++;
                printf("[%d]%d\n",bgp,child_pid);
                if(waitpid(-1,NULL,WNOHANG) &&bgp>1){
                    done++;
                    printf("[%d] Done\n",done);
            }
            background=0;
            continue;
        }
        else if(waitpid(-1,NULL,WNOHANG) &&done<bgp){
            done++;
            printf("[%d]+Done\n",done);
            if(done==bgp){
                done=0;
                bgp=0;
            }
        }
        else
            waitpid(child_pid,NULL,0);
         }
        else if(child_pid == 0){
            char* file_status;
            if (ret != -1){
                int file;
                if(ret == 2){
                    file = open(file_name, O_RDWR | O_APPEND | O_CREAT,0777);
                    ret = 1;
                }
                else
                    file = open(file_name, O_RDWR | O_TRUNC | O_CREAT,0777);

                dup2(file, ret);
            }
            int a = execvp(args[0],args);
            if(a == -1){
                 printf("Please Enter A Valid Shell Command\n");
            }
        }


    }


    return 0;
}
