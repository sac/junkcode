/* What happens if I open a file 10's of times in the same process? */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

extern int errno;

int
main(int argc, char **argv)
{
        int fd;                 /* I don't intend to do anything with this */
        int i;
        const char *filename;

        if (argc < 2) {
                printf("Usage: %s filename\n", argv[0]);
                return(1);
        }

        filename = argv[1];

        for (i = 0; i < 30; i++) {
                fd = open(filename, O_RDWR, O_APPEND);
                if (fd == -1)
                        printf("Error: %s\n", strerror(errno));
                printf("fd: %d\n", fd);
                lseek(fd, 0, SEEK_END);
                write(fd, "Hello, there you lousy fellow\n", 31);
        }
        printf("Going to sleep for an hour\n");
        return(0);
}
