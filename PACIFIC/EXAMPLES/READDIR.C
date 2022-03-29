/*
 * 	This program demonstrates the use of the intdos() function to
 *	make MS-DOS system calls. It would require some modification to
 *	work with any of the large data space memory models.
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
putdir(register char * filename)
{
	register unsigned int 	fc;
	unsigned long		total;
	union REGS		regs;
	struct FMATCH		file;

	regs.h.ah = 0x1a;
	regs.x.dx = (unsigned) &file;
	intdos(&regs, &regs);		/* set DTA to file */
	regs.h.ah = 0x4e;
	regs.x.dx = (unsigned) filename;
	regs.x.cx = 0;
	intdos(&regs, &regs);		/* get first match */
	total = fc = 0;
	while (!regs.x.cflag) {
		++fc;			/* increment file count */
		total += file.size;	/* add size to total */
		printf("%s\t", file.name);
		if (strlen(file.name) < 8)
			putchar('\t');
		printf("%8ld\n", file.size);
		regs.h.ah = 0x4f;
		intdos(&regs, &regs);	/* get next match */
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
