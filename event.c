#include <unistd.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <stdio.h>

#include "include/event.h"

// 不考虑跨平台
struct event_base *event_base_new()
{
    int epfd = epoll_create(32000);
    if (epfd == -1) return NULL;
    
    struct event_base *base = (struct event_base *)malloc(sizeof(struct event_base));
    base->epfd = epfd;
	base->nevents = INITIAL_NEVENT;
    base->events = (struct epoll_event *)malloc(sizeof(struct epoll_event) * INITIAL_NEVENT);

    return base;
}

struct event *event_new(struct event_base *base, int fd, int events, void (*cb)(int, short, void *), void *arg)
{
    struct event *ev = (struct event *)malloc(sizeof(struct event));
    ev->base = base;
    ev->ev_fd = fd;
    ev->ev_events = events;
    ev->ev_cb = cb;
    ev->ev_arg = arg;

    return ev;
}

int event_add(struct event *ev)
{
    struct event_base *base = ev->base;
    int epfd = base->epfd;
    int fd = ev->ev_fd;

	struct epoll_event epev;
    epev.events = ev->ev_events;
    epev.data.ptr = ev;
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &epev);

    return 0;
}

int event_base_dispatch(struct event_base *event_base)
{
    while (1) {
        struct epoll_event *events = event_base->events;
        int res = epoll_wait(event_base->epfd, events, event_base->nevents, -1);
        int i = 0;
        for (; i < res; ++i) {
            struct event *ev = events[i].data.ptr;
            // callback
            ev->ev_cb(ev->ev_fd, events[i].events, ev->ev_arg);
        }
    }

    return 0;
}
