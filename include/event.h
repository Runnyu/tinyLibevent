#define INITIAL_NEVENT 32
#define MAX_NEVENT 4096

// fd eventType arg
typedef void (*cb_funp)(int, short, void *);

// 不考虑跨平台
// 相当于libevent中的epollop
struct event_base {
	struct epoll_event *events;
    int epfd;
    int nevents;
};

struct event {
    struct event_base *base;
    int ev_fd;
    int ev_events;
    cb_funp ev_cb;
    void *ev_arg;
};

struct event_base *event_base_new();
struct event *event_new(struct event_base *base, int fd, int flag, cb_funp, void *arg);
int event_add(struct event *ev);
int event_del(struct event *ev);
int event_base_dispatch(struct event_base *event_base);
