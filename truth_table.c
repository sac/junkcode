/* Given N number of variables generate a truth table */

#include <stdio.h>
#include <math.h>

void next(int [], int [], int[]);

/* Limiting to 3 variables for now */
#define N 3

int
main(int argc, char **argv)
{
        int i, k;
        int base[N];
        int length[N];
        int tcase[N];

        /* Initialise the vectors */
        for (k = 1; k <= N; k++) {
                base[k] = (int)pow(2, (N - k));
                length[k] = base[k];
                tcase[k] = 0;
        }
        printf("P\tQ\tR\t!(P ^ Q) V (R V P)\n");
        for (i = 1; i <= (int)pow(2, N); i++) {
                for (k = 1; k <= N; k++)
                        printf("%d\t",tcase[k]);
                printf("%10d\n", !(tcase[1] && tcase[2]) ||
                       (tcase[3] || tcase[1]));
                next(base, length, tcase);
        }
        return(0);
}

/* Generate next set of truth values
 * Beautiful algorithm
 * */
void next(int base[], int length[], int tcase[])
{
        for (int k = 1; k <= N; k++) {
                length[k] -= 1;
                if (length[k] == 0) {
                        tcase[k] = !tcase[k];
                        length[k] = base[k];
                }
        }
}
