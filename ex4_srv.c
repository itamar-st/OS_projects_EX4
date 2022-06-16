
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>

#define MAX_LINE_LENGTH 10

void alarm_hand_srv(int sig){
    signal(SIGALRM, alarm_hand_srv);
    printf("server- got an alarm wakeup signal\n");
//    while (wait(NULL)!=-1);
    exit(0);
}
void sigUsr1_handler(int sig){
    signal(SIGUSR1, sigUsr1_handler);
    printf("server got signal from client\n");
    pid_t pid;
    pid = fork();
    if(pid == 0){
        printf("server- in child\n");
        FILE* textfile;
        char clientPid[MAX_LINE_LENGTH];
        char leftOperand[MAX_LINE_LENGTH];
        char operation[MAX_LINE_LENGTH];
        char rightOperand[MAX_LINE_LENGTH];
        if ((textfile = fopen("to_srv.txt", "r")) == NULL) {
            perror("open serverFd");
            exit(-1);
        }
        printf("server- open textfile\n");

        fgets(clientPid, MAX_LINE_LENGTH, textfile);
        int clientPidInt = atoi(clientPid);
        printf("server- clientPidInt %d\n", clientPidInt);
        fgets(leftOperand, MAX_LINE_LENGTH, textfile);
        int leftOperandInt = atoi(leftOperand);
        printf("server- leftOperand %d\n", leftOperandInt);
        fgets(operation, MAX_LINE_LENGTH, textfile);
        int operationInt = atoi(operation);
        printf("server- open operationInt %d\n", operationInt);
        fgets(rightOperand, MAX_LINE_LENGTH, textfile);
        int rightOperandInt = atoi(rightOperand);
        printf("server- open rightOperandInt %d\n", rightOperandInt);
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
        printf("server- calculate\n");
        sprintf(answerToStr, "%d", answer);
        int clientFd;
        char clientFilePath[32] = "to_client_";
        strcat(clientFilePath, clientPid);
        strcat(clientFilePath, ".txt");
        if((clientFd = open(clientFilePath,O_CREAT | O_RDWR, 0666))== -1){
            perror("open clientFile");
            exit(-1);
        }
        write(clientFd, answerToStr, strlen(answerToStr));

        fclose(textfile);
        close(clientFd);
        kill(clientPidInt, SIGUSR2);
        printf("server- send signal to client\n");

    }
}

int main(int argc, char *argv[]) {

    signal(SIGALRM, alarm_hand_srv);
    signal(SIGUSR1, sigUsr1_handler);
    alarm(60);
    while(1) {
        pause();
        alarm(60);

    }
    return 0;


}