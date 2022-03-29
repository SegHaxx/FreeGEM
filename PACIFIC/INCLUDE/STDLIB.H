/*	Standard utility functions */

#ifndef	_STDDEF
typedef	int		ptrdiff_t;	/* result type of pointer difference */
typedef	unsigned	size_t;		/* type yielded by sizeof */
typedef unsigned short	wchar_t;	/* wide char type */

#define	offsetof(ty, mem)	((int)&(((ty *)0)->mem))

#define	_STDDEF

#ifndef	NULL
#define	NULL	((void *)0)
#endif	NULL

extern int	errno;			/* system error number */
#endif	_STDDEF

#define	RAND_MAX	32767		/* max value returned by rand() */

#ifndef	_DIVTYPES
typedef struct {
	int	quot;	/* quotient */
	int	rem;	/* remainder */
} div_t;				/* div() return type */

typedef struct {
	long 	quot;	/* quotient */
	long 	rem;	/* remainder */
} ldiv_t;				/* ldiv() return type */
#define	_DIVTYPES
#endif

extern double		atof(const char *);
extern int		atoi(const char *);
extern unsigned		xtoi(const char *);
extern long		atol(const char *);
extern int		rand(void);
extern void		srand(unsigned int);
extern void *		calloc(size_t, size_t);
extern div_t		div(int numer, int denom);
extern ldiv_t		ldiv(long number, long denom);
extern unsigned long	_lrotl(unsigned long value, unsigned int shift);
extern unsigned long	_lrotr(unsigned long value, unsigned int shift);
extern unsigned int	_rotl(unsigned int value, unsigned int shift);
extern unsigned int	_rotr(unsigned int value, unsigned int shift);

#define	max(a,b)	(((a) > (b)) ? (a) : (b))
#define	min(a,b)	(((a) < (b)) ? (a) : (b))

#if	i8086 && !FLAT_MODEL
#ifndef	_STDLIB_
#define	_STDLIB_
enum {	_NO_EMS = 0, _USE_EMS };
#endif
#endif

#if	i8086 && LARGE_DATA
extern void *	malloc(size_t);
extern void	free(far void *);
extern void *	realloc(far void *, size_t);
extern void *	farmemcpy(far void *, far void *, size_t);
extern void *	farmemset(far void *, int, size_t);
extern void *	_far_alloc(size_t, char);
extern void *	_far_sbrk(size_t, char);
extern near void *	nearsbrk(size_t);
#define	farmalloc(n)	_far_alloc(n, _USE_EMS)
#define	farfree(p)	free(p)
#define	farrealloc(p,n)	realloc(p,n)
#define	_non_ems_alloc(x)	_far_alloc(x, _NO_EMS)
#define	_non_ems_sbrk(x)	_far_sbrk(x, _NO_EMS)
#else
extern void *		malloc(size_t);
extern void		free(void *);
extern void *		realloc(void *, size_t);
#define	_non_ems_alloc(x)	malloc(x)
#define	_non_ems_sbrk(x)	sbrk(x)
#if	i8086 && !FLAT_MODEL
extern far void *	_far_alloc(size_t, char);
extern far void *	_far_sbrk(size_t, char);
#define	farmalloc(n)	_far_alloc(n, _USE_EMS)
extern void		farfree(far void *);
extern far void *	farrealloc(far void *, size_t);
extern far void *	farmemcpy(far void *, far void *, size_t);
extern far void *	farmemset(far void *, int, size_t);
#endif	/* i8086 */
#endif	/* i8086 && LARGE_DATA */

extern void	abort(void);
extern void	exit(int);
extern int	atexit(void (*)(void));
extern char *	getenv(const char *);
extern char **	environ;
extern int	system(char *);
extern void	qsort(void *, size_t, size_t, int (*)(const void *, const void *));
extern void *	bsearch(const void *, void *, size_t, size_t, int(*)(const void *, const void *));
extern int	abs(int);
extern long	labs(long);
