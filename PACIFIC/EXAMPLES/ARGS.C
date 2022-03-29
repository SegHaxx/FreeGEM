#include	<stdio.h>

/*
 *	This program demonsrates use of "argc" and
 *	"argv" to iterate through the command line
 *	argument list.
 *
 *	To explore the effect of the PACC "-R" option
 *	and the PPD "_getargs() wildcard expansion"
 *	option first compile this program without "-R"
 *	or "wildcard expansion" and run ARGS *.C
 *	then compile it with wildcard expansion
 *	and run it again.
 */

main(int argc, char ** argv)
{
	while (--argc)
		puts(*++argv);
}
