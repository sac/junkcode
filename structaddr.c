/* Given the type of a structure and the address of one of its elements find
 * the address of the structure */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>

typedef struct {
    int type;
    int64_t fd_no;
    u_char gfid[16];
    u_char pargfid[16];
    char *path;
    char *bname;
    int op_ret;
    int op_errno;
} server_resolve_t;


int
main(int argc, char **argv)
{
        server_resolve_t resolv;
        server_resolve_t *base;
        server_resolve_t *addr;

        /* int *ele; */
        /* size_t offset; */

        resolv.type = 1;
        resolv.fd_no = 88;
        strncpy((char *)resolv.gfid, "AA-FFX8334r3-22", 16);
        strncpy((char *)resolv.pargfid, "BB-GGXF334r3-11", 16);
        resolv.path = "/tmp/foo/bar";
        resolv.op_errno = 99;
        resolv.op_ret = 44;

        base = (server_resolve_t *)0;
        /* ele = &base->op_ret; */
        /* offset = (ulong)ele; */
        /* addr = (server_resolve_t *)((char *)&resolv.op_ret - offset); */
        addr = (server_resolve_t *)((char *)&resolv.path - (unsigned long)&base->path);
        printf("%p op_errno: %d\n", addr, addr->op_errno);

        return(0);
}
