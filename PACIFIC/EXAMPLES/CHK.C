#include	<stdio.h>

/*
 *	CHK.C	32 bit checksum generator.  Generates
 *	and prints checksums for all files specified
 *	on the command line.
 *
 *	Usage: CHK [files]
 *
 *	Compile with PACC -R or "_getargs() wildcard expansion"
 *	if you want it to handle wildcards, e.g. CHK *.EXE
 */

unsigned long	chk;		/* checksum accumulator */
unsigned char	c;		/* character read from file */
FILE *		infile;		/* input file handle */

main(int argc, char ** argv)
{
	while(--argc) {		/* while there are more arguments */
		++argv;		/* point at next file argument */
		if (infile = fopen(argv[0], "rb")) {
			chk = 0;
			while (fread(&c, sizeof(c), 1, infile))
				chk += c;
			fclose(infile);
			printf("%s: %8.8lX\n", argv[0], chk);
		} else
			fprintf(stderr, "Cannot open %s\n", argv[0]);
	}
}
