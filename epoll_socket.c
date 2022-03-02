/* Example source:
 * https://raw.githubusercontent.com/onestraw/epoll-example/master/epoll.c */

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>

void call_server(void);
void call_client(void);

#define DEFAULT_PORT    1099
#define QUEUELENGTH     25
#define MAXEVENTS       10
#define BUFSIZE         512

int
main(int argc, char **argv)
{

    if (argc < 2) {
        printf("usage: %s s|c\n", argv[0]);
        return(0);
    }

    if (strcmp(argv[1], "s") == 0)
        call_server();
    else if(strcmp(argv[1], "c") == 0)
        call_client();
    else
        printf("usage: %s s|c\n", argv[0]);

    return(0);
}


void set_sockaddr(struct sockaddr_in *addr)
{
    bzero(addr, sizeof(struct sockaddr_in));
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = INADDR_ANY;
    addr->sin_port  = htons(DEFAULT_PORT);
    return;
}

void setnonblocking(int sockfd)
{
    if (fcntl(sockfd, F_SETFD, fcntl(sockfd, F_GETFD, 0)|O_NONBLOCK) == -1 ) {
        printf("fcntl: %s\n", strerror(errno));
    }
    return;
}


void epoll_ctl_add(int epfd, int fd, uint32_t events)
{
    struct epoll_event ev;

    ev.events = events;
    ev.data.fd = fd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
        printf("epoll_ctl: %s\n", strerror(errno));
        exit(-1);
    }
}


void call_server()
{
    int listen_sock, conn_sock;
    int i, n, nfds, epfd;
    socklen_t clisize;
    char buf[BUFSIZE];
    struct sockaddr_in servaddr;
    struct sockaddr_in cliaddr;
    struct epoll_event events[MAXEVENTS];


    listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    set_sockaddr(&servaddr);

    if (bind(listen_sock, (struct sockaddr *)&servaddr,
             sizeof(servaddr)) < 0) {
        printf("bind: %s\n", strerror(errno));
        exit(-1);
    }

    setnonblocking(listen_sock);
    listen(listen_sock, QUEUELENGTH);
    epfd = epoll_create1(0);
    epoll_ctl_add(epfd, listen_sock, EPOLLIN|EPOLLOUT|EPOLLET);
    clisize = (socklen_t)sizeof(cliaddr);

    for (;;) {
        nfds = epoll_wait(epfd, events, MAXEVENTS, -1);
        for (i = 0; i < nfds; i++) {
            if (events[i].data.fd == listen_sock) {
                    /* Call accept() if listen_sock is ready */
                conn_sock = accept(listen_sock, (struct sockaddr *)&cliaddr,
                                   &clisize);
                inet_ntop(AF_INET, (char *)&(cliaddr.sin_addr), buf, clisize);
                printf("[+] connected to %s:%d\n", buf,
                       ntohs(cliaddr.sin_port));
                setnonblocking(conn_sock);
                epoll_ctl_add(epfd, conn_sock, EPOLLIN|EPOLLET|EPOLLRDHUP|
                              EPOLLHUP);
            } else if (events[i].events & EPOLLIN) {
                    /* Handle epollin; lot of work here */
                for(;;) {
                    bzero(buf, BUFSIZE);
                    n = read(events[i].data.fd, buf, BUFSIZE);
                    if (n <= 0) {
                        break;
                    } else {
                        printf("[+] data: %s\n", buf);
                        write(events[i].data.fd, buf, BUFSIZE);
                    }
                }
            } else {
                printf("[+] Woottf!?");
            }
                /* Should we close? */
            if (events[i].events & (EPOLLRDHUP|EPOLLHUP)) {
                printf("[+] Connection closed.\n");
                epoll_ctl(epfd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
                close(events[i].data.fd);
                continue;
            }
        }
    }
    return;
}


void call_client()
{
    int n, c, sockfd;
    struct sockaddr_in serv_addr;
    char buf[BUFSIZE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    set_sockaddr(&serv_addr);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("connect: %s\n", strerror(errno));
        exit(-1);
    }

    for (;;) {
        printf("input: ");
        fgets(buf, BUFSIZE, stdin);
        c = strlen(buf) - 1;
        buf[c] = '\0';
        write(sockfd, buf, c + 1);
        bzero(buf, BUFSIZE);
        while (errno != EAGAIN && (n = read(sockfd, buf, BUFSIZE)) > 0) {
            printf("echo: %s\n", buf);
            bzero(buf, BUFSIZE);
            c -= n;
            if (c <= 0)
                break;
        }
    }
    close(sockfd);
    return;
}
