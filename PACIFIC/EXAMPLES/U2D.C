#include	<stdio.h>
#include	<sys.h>
#include	<stdlib.h>

/*
 *	Unix To DOS text file conversion utility. 
 *
 *	Converts the Unix style end-of-line sequence 0AH
 *	to MS-DOS style end-of-line sequence 0DH 0AH
 */

#define	TMPNAME	"$U2D$TMP.$$$"

/*
 *	int	fixfile(char * filename)
 *
 *	"Fix" the text file specified by "filename".
 *	Returns 0 on error, non zero otherwise.
 */

int
fixfile(char * filename)
{
	FILE *		infile;
	FILE *		outfile;
	char		ch;
	unsigned long	in, out;

	printf("u2d: %s: ", filename);
	fflush(stdout);
	infile = fopen(filename, "r");	/* try to open input file */
	if (!infile) {
		printf("not found\n");
		return 0;
	}
	outfile = fopen(TMPNAME, "w");	/* try to open scratch file */
	if (!outfile) {
		fclose(infile);
		printf("unable to open temp file %s\n", TMPNAME);
		return 1;
	}
	in = out = 0;
	while ((ch = fgetc(infile)) != EOF) {	/* while not end of file */
		if (fputc(ch, outfile) == EOF) {
			fclose(infile);
			fclose(outfile);
			printf("error writing to temp file %s\n", TMPNAME);
			return 1;
		}
		++in;
		++out;
		if (ch == '\n')
			++out;
	}
	fclose(infile);
	fclose(outfile);
	if (remove(filename) != 0) {
		printf("unable to delete %s\n", filename);
		return 1;
	}
	if (rename(TMPNAME, filename) != 0) {
		printf("unable to rename %s to %s\n", TMPNAME, filename);
		return 1;
	}
	printf("converted: %lu bytes read, %lu bytes written\n", in, out);
	return 0;

}

main(int argc, char ** argv)
{
	int	i;

	if (argc == 1) {
		argv = _getargs(0, "u2d");
		argc = _argc_;
	}
	if (argc == 1) {
		fprintf(stderr, "u2d: usage: u2d file1 file2 ...\n");
		fprintf(stderr, "u2d: wildcards, e.g. *.c are expanded\n");
		exit(1);
	}
	for (i = 1; i != argc; i++) {
		if (fixfile(argv[i])) {
			fprintf(stderr, "u2d: aborted!\n");
			exit(1);
		}
	}
}
