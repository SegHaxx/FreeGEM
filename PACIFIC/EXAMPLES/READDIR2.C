/*
 * 	This is an alternative version of readdir.c which uses msdos() to
 *	read the directory.
 *
 *	This program would require some modification to work with
 *	any of the large data space memory models.
 */

#include	<stdio.h>
#include	<string.h>
#include	<dos.h>

struct FMATCH {
	char		filler[21];
	unsigned char	attr;
	unsigned short	time, date;
	unsigned long	size;
	char		name[13];
};

static void
putdir(char * filename)
{
	register unsigned int 	fc, alreg;
	unsigned long		total;
	struct FMATCH		file;

	msdos(0x1a00, (unsigned) &file);
	alreg = msdos(0x4e00, (unsigned) filename, 0);
	total = fc = 0;
	while (!alreg) {
		++fc;			/* increment total file count */
		total += file.size;	/* add filesize to running total */
		printf("%s\t", file.name);
		if (strlen(file.name) < 8)
			putchar('\t');
		printf("%8ld\n", file.size);
		alreg = msdos(0x4f00);
	}
	printf("%ld bytes in %d files\n", total, fc);
}

main()
{
	char	buf[128];

	do {
		printf("Wildcard: ");
		gets(buf);
		if (*buf)
			putdir(buf);
	} while (*buf);
}
