#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <errno.h>
#include <pthread.h>

#define BUF_MAX_SIZE 1024

int sockfd;
void *recv_thread(void *arg)
{
    int n = 0;
    char buf[BUF_MAX_SIZE];
    while ((n = read(sockfd, buf, BUF_MAX_SIZE)) > 0) {
        write(STDOUT_FILENO, buf, n);
    }
}

void *send_thread(void *arg)
{
    int n = 0;
    char buf[BUF_MAX_SIZE];
    while ((n = read(STDIN_FILENO, buf, BUF_MAX_SIZE)) > 0) {
        write(sockfd, buf, BUF_MAX_SIZE);
    }
}

int main(int argc, char **argv)
{
    pthread_t tid1, tid2;
    struct sockaddr_in servaddr;
    void *ret;

    char *ip = "127.0.0.1";
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8888);
    inet_pton(AF_INET, ip, &servaddr.sin_addr);

    connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    pthread_create(&tid1, NULL, send_thread, NULL);
    pthread_create(&tid2, NULL, recv_thread, NULL);
    pthread_join(tid1, &ret);
    pthread_join(tid2, &ret);

    close(sockfd);
}
