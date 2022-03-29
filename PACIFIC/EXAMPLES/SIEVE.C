#include	<stdio.h>
#include	<stdlib.h>

#define	TRUE	1
#define	FALSE	0
#define	SIZE	8190

char	flags[SIZE+1];

main()
{
	register int	k, j, i, count, prime;

	printf("100 iterations of sieve\n");
	for(i = 1 ; i <= 100 ; i++) {
		count = 0;
		for(j = 0 ; j <= SIZE ; j++)
			flags[j] = TRUE;
		for(j = 0 ; j <= SIZE ; j++) {
			if(flags[j]) {
				prime = j + j + 3;
				k = j + prime;
				while(k <= SIZE) {
					flags[k] = FALSE;
					k += prime;
				}
				count = count + 1;
			}
		}
	}
	printf("\n%d primes\n", count);
	exit(0);
}
