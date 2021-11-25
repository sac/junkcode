/* Given a filename, determine the file type.
   This does not determine based on magic(5) but the st_mode field.
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>



int
main(int argc, char **argv)
{
        const char *filename;
        int ret;
        struct stat sb;

        if (argc < 2) {
                printf("Usage: %s <filename>\n", argv[0]);
                return(1);
        }
        filename = argv[1];
        ret = lstat(filename, &sb);

        if (ret != 0) {
                printf("%s\n", strerror(errno));
                return ret;
        }

        switch (sb.st_mode & S_IFMT) {
        case S_IFIFO:
                printf("Named pipe (fifo).\n");
                break;
        case S_IFDIR:
                printf("Directory.\n");
                break;
        case S_IFCHR:
                printf("Character special file.\n");
                break;
        case S_IFBLK:
                printf("Block special file.\n");
                break;
        case S_IFLNK:
                printf("Symbolic link.\n");
                break;
        case S_IFSOCK:
                printf("Socket.\n");
                break;
        case S_IFREG:
                printf("Regular file.\n");
                break;
        default:
                printf("Unknown file type.\n");
        }

                                /* OR */

/*       if (sb.st_mode & S_IFIFO)
               printf("Named pipe (fifo).\n");
       else if (sb.st_mode & S_IFDIR)
               printf("Directory.\n");
       else if (sb.st_mode & S_IFCHR)
               printf("Character special file.\n");
       else if (sb.st_mode & S_IFBLK)
               printf("Block special file.\n");
       else if (sb.st_mode & S_IFLNK)
               printf("Symbolic link.\n");
       else if (sb.st_mode & S_IFSOCK)
               printf("Socket.\n");
       else if (sb.st_mode & S_IFREG)
               printf("Regular file.\n");
       else
               printf("Unknown file type.\n");
*/
       return(0);
}

