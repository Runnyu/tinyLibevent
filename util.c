#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <netinet/in.h>

int set_socket_nonblocking(int fd)
{
    int flags;
    if ((flags = fcntl(fd, F_GETFL, NULL)) < 0) return -1;
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) return -1;

    return 0;
}

int tcp_server_init(int port, int listen_num)
{
    int listener = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = 0;
	sin.sin_port = htons(port);

	if (bind(listener, (struct sockaddr *)&sin, sizeof(sin)) < 0) return -1;
	if (listen(listener, listen_num) < 0) return -1;
    if (set_socket_nonblocking(listener) == -1) return -1;

    return listener;
}


