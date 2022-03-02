/* No changes from earlier version, just fancy dynamic allocation */
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#define MAXBUF 1024
#define MAXEVENTS 5

/* Ignoring error checking throughout */

int
main(int argc, char **argv)
{
    int epfd, ready;
    int s, j, fd, open_fds;
    struct epoll_event *ev;

    char *buf;

    if (argc < 2) {
            /* Pipes for this example */
        printf("usage: %s pipe1 [pipe2 pipe 3 ...]\n", argv[0]);
        return(1);
    }

    epfd = epoll_create(argc - 1);
    buf = (char *)malloc(MAXBUF);
    ev = (struct epoll_event *)malloc(sizeof(struct epoll_event));

        /* Open the files and add the fds to epoll instance */
    for (j = 1; j < argc; j++) {
        fd = open(argv[j], O_RDONLY);
        printf("Opened %s on fd: %d\n", argv[j], fd);
        ev->events = EPOLLIN;
        ev->data.fd = fd;
        epoll_ctl(epfd, EPOLL_CTL_ADD, fd, ev);
    }

    open_fds = argc - 1;

        /* Reuse the variable ev (I'm a miser) */
    ev = (struct epoll_event *)calloc(MAXEVENTS, sizeof(struct epoll_event));

    while (open_fds > 0) {
        ready = epoll_wait(epfd, ev, MAXEVENTS, -1);
        if (ready == -1) {
            if (errno == EINTR)
                continue;
            else
                exit(-1);
        }

        printf("ready = %d\n", ready);

        for (j = 0; j < ready; j++) {
            printf("fd = %d, events: %s%s%s\n", ev->data.fd,
                   (ev->events & EPOLLIN)? "EPOLLIN ": "",
                   (ev->events & EPOLLHUP)? "EPOLLHUP": "",
                   (ev->events & EPOLLERR)? "EPOLLERR": "");

                /* Deal with EPOLLIN */
            if (ev->events & EPOLLIN) {
                s = read(ev->data.fd, buf, MAXBUF);
                if (s == -1) {
                    printf("read: %s\n", strerror(errno));
                    exit(-1);
                }
                printf("read %d bytes: %.*s\n", s, s, buf);
            } else if (ev->events & (EPOLLHUP|EPOLLERR)) {
                    /* Lots of bytes to read or the write end closed.
                     * close(2) the fd */
                close(ev->data.fd);
                open_fds--;
                ev++;
            }
        }
    }
    printf("All fds are closed bye.\n");
    return(0);
}
