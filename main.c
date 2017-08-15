#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/epoll.h>

#include "include/event.h" 
#include "include/util.h" 

#define BUF_MAX_SIZE 1024

void socket_read_cb(int fd, short events, void *arg)
{
    ssize_t n;
    socklen_t len;
    struct sockaddr_in addr;
    char buf[BUF_MAX_SIZE], ip[20];
    getpeername(fd, (struct sockaddr *)&addr, &len);
    inet_ntop(AF_INET, &addr.sin_addr, ip, sizeof(ip)); 

    n = read(fd, buf, BUF_MAX_SIZE);
    if (n < 0 && errno == EAGAIN) return;
    if (n < 0) {
        printf("socket close %s:%d\n", ip, addr.sin_port);
        close(fd);
        return;
    }

    printf("recv from %s:%d msg: %s\n", ip, addr.sin_port, buf);
    write(fd, buf, n);
}

void accept_cb(int fd, short events, void *arg)
{
	int sockfd;
	struct sockaddr_in client;
	socklen_t len = sizeof(client);

	sockfd = accept(fd, (struct sockaddr *)&client, &len);
    set_socket_nonblocking(sockfd);

    char ip[20];
    struct sockaddr_in addr;
    getpeername(sockfd, (struct sockaddr *)&addr, &len);
    inet_ntop(AF_INET, &addr.sin_addr, ip, sizeof(ip)); 
    printf("acpt from %s:%d\n", ip, addr.sin_port);

    struct event_base *base = (struct event_base *)arg;
	struct event *ev_client = event_new(base, sockfd, EPOLLIN | EPOLLET, socket_read_cb, NULL);
	event_add(ev_client);
}

int main(int argc, char **argv)
{
    int listener = tcp_server_init(8888, 1000);
	if (listener == -1) {
		perror("tcp_server_init error");
		return -1;
	}
	struct event_base *base = event_base_new();
	struct event *ev_listen = event_new(base, listener, EPOLLIN | EPOLLET, accept_cb, (void *)base);
	event_add(ev_listen);
	event_base_dispatch(base);

    return 0;
}
