/*
 *	This program demonstrates the use of the command line arguments
 *	via argc & argv, as well as the use of the getenv() function to
 *	access MS-DOS shell variables
 *
 *	Example:  printenv PATH
 *
 */

#include	<stdio.h>
#include	<stdlib.h>

main(argc, argv)
int	argc;
char	**argv;
{
	register char	*varptr;

	while (--argc) {
		if (varptr = getenv(*++argv))
			printf("%s=%s\n", *argv, varptr);
		else
			printf("%s not defined\n", *argv);
	}
}
