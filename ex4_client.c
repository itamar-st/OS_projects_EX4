#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define BUFF_SIZE 30
void alarm_hand(int sig){
    signal(SIGALRM, alarm_hand);
    printf("got an alarm wakeup signal\n");
}

int main(int argc, char *argv[]) {
    if(argc != 5){
        printf("ERROR_FROM_EX4\n");
    }
    int fd;
    int clientFd;
    signal(SIGALRM, alarm_hand);
    srand(time(NULL));
    int counter = 0;
    int r;
    char pidAsStr[10];
    pid_t pid = getpid();
    sprintf(pidAsStr, "%d", pid);
    while((fd = open("to_srv.txt",O_CREAT |O_RDWR, 0666))==-1){
        counter++;
        r = rand() % 5 + 1;
        alarm(r);
        if(counter == 10){
            printf("%d - tried to write 10 times and not avalible\n", pid);
            exit(1);
        }
    }
    write(fd, pidAsStr, strlen(pidAsStr));
    write(fd, "\n", 1);
    write(fd, argv[2], strlen(argv[2]));
    write(fd, "\n", 1);
    write(fd, argv[3], strlen(argv[3]));
    write(fd, "\n", 1);
    write(fd, argv[4], strlen(argv[4]));
    write(fd, "\n", 1);
    close(fd);

    kill(atoi(argv[1]), SIGUSR1);
    alarm(30);
    pause();

    char clientFileName[32] = "to_client";
    strcat(clientFileName, pidAsStr);
    if((clientFd = open(clientFileName,O_RDONLY)) == -1){
        perror("open clientFd");
        printf("Client closed because no response was received from the server for 30 seconds");
        exit(-1);
    }
    char answer[32];
    if(read(clientFd, answer, sizeof(answer)) == -1){
        perror("read clientFd");
        exit(-1);
    }
    printf("%s",answer);






    return 0;


}
