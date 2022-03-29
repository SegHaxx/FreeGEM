/*
 *	Declarations for Unix style low-level I/O functions.
 */

#ifndef	_STDDEF
typedef	int		ptrdiff_t;	/* result type of pointer difference */
typedef	unsigned	size_t;		/* type yielded by sizeof */
typedef	unsigned short	wchar_t;	/* wide char type */
#define	_STDDEF
#define	offsetof(ty, mem)	((int)&(((ty *)0)->mem))
#endif	_STDDEF

#ifndef	NULL
#define	NULL	((void *)0)
#endif	NULL

#ifndef	_UNIXIO
#define	_UNIXIO
struct utimbuf
{
	long	actime;
	long	modtime;
};
#endif

extern int	errno;			/* system error number */

extern int	open(const char *, int, ...);
extern int	close(int);
extern int	creat(const char *, int);
extern int	dup(int);
extern int	dup2(int, int);
extern long	lseek(int, long, int);
extern size_t	read(int, void *, size_t);
extern int	unlink(const char *);
extern size_t	write(int, const void *, size_t);
extern int	isatty(int);
extern int	chmod(const char *, unsigned);
extern int	utime(const char *, struct utimbuf *);
#if	unix || ATDOS
extern int	umask(int);
extern int	chown(const char *, unsigned);
extern int	ioctl(int, int, void *);
extern int	pipe(int *);
#endif
extern int	access(const char *, int);
