/*
 *	Structures for the dos interface routines:
 *		intdos()
 *		intdosx()
 *		int86()
 *		int86x()
 *		segread()
 *		intr()
 *		driver()
 *		driverx()
 */

struct WORDREGS {
	unsigned int ax;
	unsigned int bx;
	unsigned int cx;
	unsigned int dx;
	unsigned int si;
	unsigned int di;
	unsigned int cflag;
	unsigned int psw;
};

struct BYTEREGS {
	unsigned char al, ah;
	unsigned char bl, bh;
	unsigned char cl, ch;
	unsigned char dl, dh;
};

union REGS {
	struct WORDREGS x;
	struct BYTEREGS h;
};

struct SREGS {
	unsigned int es;
	unsigned int cs;
	unsigned int ss;
	unsigned int ds;
};

struct WORDREGPACK {
	unsigned int	ax;
	unsigned int	bx;
	unsigned int	cx;
	unsigned int	dx;
	unsigned int	bp;
	unsigned int	si;
	unsigned int	di;
	unsigned int	ds;
	unsigned int	es;
	unsigned int	flags;
};

union REGPACK {
	struct WORDREGPACK	x;
	struct BYTEREGS		h;
};

/*	The following macros split a long pointer (large model) into
 *	offset and segment parts.
 */

#define FP_OFF(ptr)	((unsigned short)(ptr))
#define FP_SEG(ptr)	(((long)(far void *)(ptr)) >> 16)

/*	construct a far pointer from segment and offset */

#if	LARGE_DATA
#define	FP_CONSTRUCT(seg, offs)	((void *)(((long)(seg) << 16) + (offs)))
#else
#define	FP_CONSTRUCT(seg, offs)	((far void *)(((long)(seg) << 16) + (offs)))
#endif

#define	MK_FP(seg, offs)	FP_CONSTRUCT(seg, offs)

extern int	intdos(union REGS *, union REGS *);
extern int	intdosx(union REGS *, union REGS *, struct SREGS *);
extern int	int86x(int, union REGS *, union REGS *, struct SREGS *);
extern int	int86(int, union REGS *, union REGS *);
extern int	driverx(far void *, union REGS *, struct SREGS *);
extern int	driver(far void *, union REGS *);
extern void	intr(int, union REGPACK *);
extern int	segread(struct SREGS *);
extern long	msdos(int, ...);
extern long	msdoscx(int, ...);
typedef	int (*	__critf_)(unsigned char, unsigned char);
extern __critf_	set_crithand(__critf_);
extern int	isnec98(void);
extern int	_dos_getftime(int, unsigned short *, unsigned short *);
extern unsigned char	__in_crithand__;
extern void	nosound(void);
extern void	sound(unsigned int frequency);

/*
 *	Byte port I/O macros
 */

#define	inp(p)		(*(port unsigned char *)(p))
#define	inportb(p)	(*(port unsigned char *)(p))
#define	outp(p,v)	(*(port unsigned char *)(p) = (v))
#define	outportb(p,v)	(*(port unsigned char *)(p) = (v))

/*
 *	Word port I/O macros
 */

#define	inpw(p)		(*(port unsigned short *)(p))
#define	inport(p)	(*(port unsigned short *)(p))
#define	outpw(p,v)	(*(port unsigned short *)(p) = (v))
#define	outport(p,v)	(*(port unsigned short *)(p) = (v))

/*
 *	End of DOS.H
 */
