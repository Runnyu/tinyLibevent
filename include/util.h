#ifndef __TINY_LIBEVENT_UTIL_H__
#define __TINY_LIBEVENT_UTIL_H__

int set_socket_nonblocking(int fd);
int tcp_server_init(int port, int listen_num);

#endif
