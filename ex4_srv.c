
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 10

void alarm_hand(int sig){
    signal(SIGALRM, alarm_hand);
    printf("got an alarm wakeup signal\n");
}
void sigUsr1_handler(int sig){
    signal(SIGUSR1, sigUsr1_handler);
    printf("server got signal from client\n");
    pid_t pid;
    pid = fork();
    if(pid == 0){
        FILE* textfile;
        char clientPid[MAX_LINE_LENGTH];
        char leftOperand[MAX_LINE_LENGTH];
        char operation[MAX_LINE_LENGTH];
        char rightOperand[MAX_LINE_LENGTH];
        if ((textfile = fopen("to_srv.txt", "r")) == NULL) {
            perror("open serverFd");
            exit(-1);
        }

        fgets(clientPid, MAX_LINE_LENGTH, textfile);
        int clientPidInt = atoi(clientPid);
        fgets(leftOperand, MAX_LINE_LENGTH, textfile);
        int leftOperandInt = atoi(clientPid);
        fgets(operation, MAX_LINE_LENGTH, textfile);
        int operationInt = atoi(clientPid);
        fgets(rightOperand, MAX_LINE_LENGTH, textfile);
        int rightOperandInt = atoi(clientPid);
        int answer;
        char answerToStr[32];
        if (rightOperandInt == 0 && operationInt == 3){
            strcpy(answerToStr, "CANNOT_DIVIDE_BY_ZERO\n");
        }
        switch (operationInt) {
            case 1: answer = leftOperandInt + rightOperandInt;
                break;
            case 2: answer = leftOperandInt - rightOperandInt;
                break;
            case 3: answer = leftOperandInt * rightOperandInt;
                break;
            case 4: answer = leftOperandInt / rightOperandInt;
                break;
        }
        sprintf(answerToStr, "%d", answer);
        int clientFd;
        char clientFile[32] = "to_client_";
        strcat(clientFile, clientPid);
        strcat(clientFile, ".txt");
        if((clientFd = open(clientFile,O_CREAT |O_RDWR, 0666))== -1){
            perror("open clientFile");
            exit(-1);
        }
        write(clientFd, answerToStr, strlen(answerToStr));

        fclose(textfile);

    }
}

int main(int argc, char *argv[]) {

    signal(SIGALRM, alarm_hand);
    alarm(60);
    while(1) {
        pause();


        return 0;
    }


}