/*
 *	HEXD:	A HEX Dump utility.
 *
 *	This program will print a hex and ascii dump of any
 *	input file(s) passed to it.  It may either be used
 *	to process one or more input files passed on the
 *	command line, e.g. HEXD afile bfile, or as a filter
 *	to process standard input.  Usage is:
 *
 *	HEXD [files]
 *
 *	where [files] represents 0 or more input files.
 *
 *	If compiled with the PACC option "-R" or the PPD
 *	option "_getargs() wildcard expansion", HEXD will
 *	be able to handle wildcards, e.g. HEXD *.BIN
 */

#include	<stdio.h>
#include	<stdlib.h>

FILE	*infile;

static void	print_buf();
static void	do_dump();

static char	cset[128];

static void
print_buf(buffer, items, filepos)
unsigned char *	buffer;
int		items;
long		filepos;
{
	int	bufpos;
	char	buf[128];
	char *	bp;

	bp = buf;
	bp += sprintf(bp, "%8.8lX:", filepos);
	for (bufpos = 0; bufpos != items; bufpos++)
		bp += sprintf(bp, "%2.2X ", *buffer++);
	for (bufpos = items; bufpos != 16; bufpos++) {
		*bp++ = ' ';
		*bp++ = ' ';
		*bp++ = ' ';
	}
	buffer -= items;
	for (bufpos = 0; bufpos != items; bufpos++)
		*bp++ = cset[*buffer++ & 0x7F];
	*bp = 0;
	puts(buf);
}

static void
do_dump()
{
	unsigned char	buffer[16];
	long		filepos;
	int		items;

	filepos = 0;
	while ((items = fread(buffer, sizeof(char), 16, infile)) == 16) {
		print_buf(buffer, items, filepos);
		filepos += 16;
	}
	if (items > 0 && items < 16)
		print_buf(buffer, items, filepos);
}

main(argc, argv)
int	argc;
char	**argv;
{
	int	argnum;
	int	c;

	if (!argv[0])			/* argv[0] may not be the program */
		argv[0] = "hexd";	/* name under some versions of DOS */
	for (c = 0; c != 32; c++)
		cset[c] = '.';
	for ( ; c != 127; c++)
		cset[c] = c;
	cset[c] = '.';
	if (argc == 1) {
		printf("<standard input>\n");
		infile = stdin;
		do_dump();
	} else {
		for (argnum = 1; argnum != argc; argnum++) {
			if (infile = fopen(argv[argnum], "rb")) {
				puts(argv[argnum]);
				do_dump();
				fclose(infile);
			} else {
				fprintf(stderr,"%s: unable to open %s\n",
					argv[0], argv[argnum]);
				exit(1);
			}
		}
	}
	exit(0);
}
