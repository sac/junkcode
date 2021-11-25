/* Given the type of a structure and the address of one of its elements find
 * the address of the structure */

#include <stdio.h>
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
        server_resolve_t *base;
        server_resolve_t *addr;
        /* int *ele; */
        /* size_t offset; */

        server_resolve_t resolv = {
                .type =  1,
                .fd_no = 8,
                .gfid  = "aa-jnhskmk-lk",
                .pargfid = "bb-iknkjex-981",
                .path    = "/tmp/foo/bar/baz",
                .op_errno = 81,
                .op_ret   = 3,
        };

        base = (server_resolve_t *)0;
        /* ele = &base->op_ret; */
        /* offset = (ulong)ele; */
        /* addr = (server_resolve_t *)((char *)&resolv.op_ret - offset); */
        addr = (server_resolve_t *)((char *)&resolv.path - (unsigned long)&base->path);
        printf("%p op_errno: %d\n", addr, addr->op_errno);

        return(0);
}
