/*
 * Demo for epoll
 * writed by panzhh
 * 2013.11.15
 */

#include <stdio.h>  
#include <string.h>
#include <stdlib.h>

#include <fcntl.h>  
#include <unistd.h>  

#include <sys/epoll.h>

#include "myepoll.h"

int add_myevent(int epfd, MYEVENT *evp)
{
	struct epoll_event event = {0,{0}};
	event.events = evp->events;
	event.data.ptr = evp;
	return epoll_ctl (epfd, EPOLL_CTL_ADD, evp->fd, &event);
}

int del_myevent(int epfd, MYEVENT *evp)
{
	struct epoll_event event={0,{0}};
	return epoll_ctl (epfd, EPOLL_CTL_ADD, evp->fd, &event);
}

int mod_myevent(int epfd, MYEVENT *evp)
{
	struct epoll_event event = {0,{0}};
	event.events = evp->events;
	event.data.ptr = evp;
	return epoll_ctl (epfd, EPOLL_CTL_MOD, evp->fd, &event);
}

void init_myevent(MYEVENT *evp, int fd, unsigned int events, HANDLER handler, void *arg)
{
	evp->fd		= fd;
	evp->events	= events;
	evp->handler	= handler;
	evp->arg	= arg;
}

