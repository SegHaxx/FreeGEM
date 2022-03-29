#include	<stat.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

/*
 *	USAGE.C		Colin Weaver, HI-TECH Software, 1992
 *
 *	MS-DOS file space/blocks usage utility.  Scans all files
 *	in one or more directories and works out how many bytes
 *	and allocation blocks of disk space have been used.
 *
 *	This utility can recursively scan directories, thereby
 *	giving the entire space usage for a directory tree.
 *
 *	Usage [-Bblocksize] [-D] [-V] [-R] [files]
 *
 *		-Bblocksize	set disk block size (default 1024)
 *		-D		account for space used by directories
 *		-Q		quiet mode: don't list files or directories
 *		-R		recursively scan directories (turns on -D)
 *		-V		verbose: list files as they are scanned
 *
 *	Any other argument will be treated as a wildcard.
 */

#ifndef	BOOL
#define	BOOL	unsigned char
#define	FALSE	0
#define	TRUE	1
#endif

/*
 *	Parameters and settings.
 */

#if	DOS
#define	DIRSEP		'\\'
#define	WILDCARD	"*.*"
#else
#define	DIRSEP		'/'
#define	WILDCARD	"*"
#endif

static char		dirsep[] = { DIRSEP, 0 };

static unsigned int	block_size = 1024;	/* size of a disk block */
static BOOL		verbose = FALSE;	/* verbose mode ? */
static BOOL		add_dirs = FALSE;	/* account for directories */
static BOOL		recurse = FALSE;	/* recursive scan ? */
static BOOL		quiet = FALSE;		/* quiet scan ? */
static char		wildcard[1024];		/* current wildcard */

/*
 *	Global totals
 */

static unsigned long	total_size = 0;		/* total size in bytes */
static unsigned long	total_blocks = 0;	/* total blocks */
static unsigned long	total_files = 0;	/* total number of files */
static unsigned int	total_dirs = 0;		/* number of dirs scanned */

/*
 *	void	scan(void)
 *
 *	Scan a directory using the current wildcard.
 */

static void
scan(void)
{
static	struct dirbuf *	dir;			/* directory entry */
static	unsigned int	files;
static	unsigned long	file_size;		/* size of current file */
static	unsigned long	file_blocks;		/* blocks in current file */
static	unsigned long	dir_size;		/* current directory size */
static	unsigned long	dir_blocks;		/* current directory blocks */
static	unsigned int	s_namesize;
static	unsigned int	s_bufsize;
	char *		s_wcard;
	char *		s_buf;
	unsigned int	s_ofs;
	unsigned int	s_count;

	dir_size = dir_blocks = 0;		/* zap directory totals */
	files = 0;
	dir = ffirst(wildcard);
	if (!dir) {
		if (!(quiet && recurse))
			fprintf(stderr, "No files match %s\n", wildcard);
		return;
	}
	if (!quiet) {
		if (strlen(wildcard) <= 24) {
			printf("%-24s: ", wildcard);
			if (verbose)
				putchar('\n');
			else
				fflush(stdout);
		} else {
			printf("%s:\n", wildcard);
			if (!verbose) {
				printf("%26s", " ");
				fflush(stdout);
			}
		}
	}
	s_buf = NULL;
	s_bufsize = s_count = s_ofs = 0;
	for (; dir; dir = fnext()) {
		if (dir->di_stat.st_mode & S_IFDIR) { 	/* directory ? */
			if (!add_dirs)
				continue;
			if (dir->di_name[0] == '.' && (dir->di_name[1] == 0 || dir->di_name[1] == '.'))
				continue;
			if (recurse) {
				s_namesize = strlen(dir->di_name) + 1;
				if ((s_ofs + s_namesize) > s_bufsize) {
					s_bufsize += 512;
					s_buf = realloc(s_buf, s_bufsize);
					if (!s_buf) {
						fprintf(stderr, "Abort: unable to allocate any more memory\n");
						exit(1);
					}
				}
				strcpy(s_buf + s_ofs, dir->di_name);
				s_ofs += s_namesize;
				++s_count;
			}
		}
		file_size = dir->di_stat.st_size;
		file_blocks = (file_size + block_size - 1) / block_size;
		dir_size += file_size;
		dir_blocks += file_blocks;
		++files;
		if (verbose && !quiet)
			printf("%-12s | %8lu bytes | %5lu blocks%s\n",
				dir->di_name, file_size, file_blocks,
				(dir->di_stat.st_mode & S_IFDIR)? " <DIR>": "");
	}
	if (!quiet)
		printf("Total: %8lu bytes, %5lu blocks, %d files\n",
			dir_size, dir_blocks, files);
	total_files += files;
	total_size += dir_size;
	total_blocks += dir_blocks;
 	++total_dirs;
	if (quiet) {
		printf("%lu files, %lu blocks, %lu bytes\r",
			total_files, total_blocks, total_size);
		fflush(stdout);
	}
	if (recurse && s_count) {
		if (s_wcard = strrchr(wildcard, DIRSEP))
			++s_wcard;
		else
			s_wcard = wildcard;
		s_ofs = 0;
		while (s_count--) {
			strcat(strcat(strcpy(s_wcard, s_buf + s_ofs), dirsep), WILDCARD);
			s_ofs += strlen(s_buf + s_ofs) + 1;
			scan();
		}
		free(s_buf);
	}
}

/*
 *	void	bad_arg(char * arg)
 *
 *	Complain about a bad argument.
 */

static void
bad_arg(char * arg)
{
	fprintf(stderr, "Bad argument ignored: \"%s\"\n", arg);
}

/*
 *	main(int argc, char ** argv)
 *
 *	Handle command line arguments
 */

int
main(int argc, char ** argv)
{
	unsigned int	bs;
	BOOL		scan_done;

	scan_done = FALSE;
	while (*++argv) {
		if (argv[0][0] == '-') {
			switch(argv[0][1]) {
			case 'b':
			case 'B':
				if (scan_done)
					fprintf(stderr, "Can't change block size now: a directory has already been scanned!\n");
				else if (sscanf(argv[0] + 2, "%d", &bs) != 1)
					bad_arg(argv[0]);
				else if (bs < 1)
					bad_arg(argv[0]);
				else
					block_size = bs;
				break;
			case 'r':
			case 'R':
				recurse = TRUE;
			case 'd':
			case 'D':
				add_dirs = TRUE;
				break;
			case 'v':
			case 'V':
				verbose = TRUE;
				break;
			case 'q':
			case 'Q':
				quiet = TRUE;
				break;
			default:
				bad_arg(argv[0]);
				break;
			}
		} else {
			scan_done = TRUE;
			strcpy(wildcard, argv[0]);
			scan();
		}
	}
	if (!scan_done) {
		strcpy(wildcard, WILDCARD);
		scan();
	}
	if (total_dirs > 1 && !quiet) {
		printf("%u directories scanned:\n", total_dirs);
		printf("Total: %8lu bytes, %5lu blocks, %lu files\n",
			total_size, total_blocks, total_files);
	}
	if (quiet)
		putchar('\n');
}
