#include	<stdio.h>

#if	!LARGE_MODEL
#error	This program must be compiled with the LARGE memory model
#endif

extern char *	_fullpath;

main()
{
	printf("Full path = %s\n", _fullpath);
}
