#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <time.h>
#include <stdbool.h>

#define MAX_LIMIT 1000
int num;
//PROCESS LIST STRUCTURE
typedef struct processlist{
        char procName[50];
        int proc_id;
        time_t startTime;
        time_t endTime;
        bool isActive;
}proc_list;

struct processlist plist[MAX_LIMIT];
int plist_index=0;


//ADDITION
float addition(char *str){

        float substr = 0;
        float sum = 0;
        while(str != NULL){
        substr = atof(str);
        sum += substr;
        str = strtok(NULL," ");
        }
        return sum;
}
//SUBTRACTION
float subtract(char *str){
        float substr = 0;
        float res = atof(str);
        str = strtok(NULL," ");
        while(str != NULL){
        substr = atof(str);
        res -= substr;
        str = strtok(NULL," ");
        }
        return res;
}
//MULTIPLICATION
float multiply(char *str){
        float substr = 0;
        float res = 1;
        while(str != NULL){
        substr = atoi(str);
        res = res*substr;
        str = strtok(NULL," ");
        }
        return res;
}
//DIVISION
float divide(char *str){
        float substr = 0;
        float res = atof(str);
        str = strtok(NULL," ");
        while(str != NULL){
        substr = atof(str);
        res = res/substr;
        str = strtok(NULL," ");
        }
        return res;
}
//RUN PROGRAM
void run(char *str){
    char pname[50];
    sprintf(pname, "%s", str);
    char buff[MAX_LIMIT];
    //REMOVING ENTER FROM THE GIVEN STRING
    //if(str[strlen(str)-1] == '\n'){
    //    str[strlen(str)-1] = '\0';
    //}
    if(str == NULL || str == ""){
            write(STDOUT_FILENO, "Incomplete command.", 19);
    }
        int this_pipe[2];
        pipe(this_pipe);
        int pid = fork();
        

    if(pid==-1){
        perror("Error at fork: ");
        exit(1);
    }
    //CHILD
    else if(pid == 0){//Run();
            close(this_pipe[0]);
            int ret=0;
            char * args[] = {str,NULL};
            int n = sprintf(buff, "%d", getpid());
            write(this_pipe[1], buff, n);
            ret = execvp(str, args);
        if (ret == -1) {
            perror("Error at exec: ");
            exit(1);
        }
    }
    //PARENT
    else{
        close(this_pipe[1]);
        int c_pid;
        int n = read(this_pipe[0],buff, MAX_LIMIT);
        n = sscanf(buff, "%d", &c_pid);
        plist[plist_index].proc_id = c_pid;
        sprintf(plist[plist_index].procName, "%s", pname);
        plist[plist_index].isActive = true;
        time(&plist[plist_index].startTime);
        plist_index++;
        }
}




int main()
{
        char buff[MAX_LIMIT];
        char *tok = NULL;
        float result=0;
        char user_input[500] = " ";
        while(1){
        num = write(STDOUT_FILENO,"\nCommand: ",10);
        if(num<0){
        perror("Error at write: ");
        exit(1);
        }
        num = read(STDIN_FILENO, user_input, sizeof(user_input));
        if(num<0){
        perror("Error at input: ");
        exit(0);
        }
        //REMOVING ENTER FROM THE GIVEN STRING
    if(user_input[strlen(user_input)-1] == '\n'){
        user_input[strlen(user_input)-1] = '\0';
    }

        tok = strtok(user_input, " "); //HAS THE INITIAL COMMAND


if(strcmp(tok, "add")==0){
        tok = strtok(NULL, " ");
        result = addition(tok);
        int input_length = sprintf(user_input,"%.2f",result);
        write(STDOUT_FILENO, "Result: ", 8);
        write(STDOUT_FILENO,user_input,input_length);
}
else if(strcmp(tok, "sub")==0){
        tok = strtok(NULL, " ");
        result =subtract(tok);
        int input_length = sprintf(user_input,"%.2f",result);
        write(STDOUT_FILENO, "Result: ", 8);
        write(STDOUT_FILENO,user_input,input_length);
}
else if(strcmp(tok, "mul")==0){
        tok = strtok(NULL, " ");
        result = multiply(tok);
        int input_length = sprintf(user_input,"%.2f",result);
        write(STDOUT_FILENO, "Result: ", 8);
        write(STDOUT_FILENO,user_input,input_length);
}
else if(strcmp(tok, "div")==0){
        tok = strtok(NULL, " ");
        result = divide(tok);
        int input_length = sprintf(user_input,"%.2f",result);
        write(STDOUT_FILENO, "Result: ", 8);
        write(STDOUT_FILENO,user_input,input_length);
}
else if(strcmp(tok, "run")==0){
        tok = strtok(NULL, " ");
        run(tok);
}
else if(strcmp(tok, "list")==0){
        char buff[MAX_LIMIT];
        int n = sprintf(buff, "Total number of processes: %d\n", plist_index);
        write(STDOUT_FILENO, buff, n);
        n = sprintf(buff, "\nName\tpid\tisActive\tStart Time\n");
        write(STDOUT_FILENO, buff, n);
        

        for(int i=0; i<plist_index; i++){
        char stime[9];
        strftime(stime, 9, "%X", gmtime(&plist[i].startTime));
        n = sprintf(buff, "\n%s\t%d\t%d\t\t%s",plist[i].procName, plist[i].proc_id, plist[i].isActive, stime);
        write(STDOUT_FILENO, buff, n);
        }
}
else if(strcmp(tok, "exit")==0){
        write(STDOUT_FILENO, "Exiting....", 12);
        exit(0);
}
}
}
