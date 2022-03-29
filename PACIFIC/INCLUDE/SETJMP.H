#ifdef	z80
typedef	int	jmp_buf[5];
#endif

#ifdef	i8086
typedef	int	jmp_buf[8];
#endif

#ifdef	i8096
typedef	int	jmp_buf[10];
#endif

#ifdef	m68k
typedef	int	jmp_buf[10];
#endif

#ifdef	__psxa
typedef	int	jmp_buf[4];
#endif

#ifdef	m6809
typedef	int	jmp_buf[4];
#endif

#ifdef	m6800
typedef	int	jmp_buf[2];
#endif

#ifdef	i8051
typedef int	jmp_buf[4];
#endif

#ifdef	h8300
typedef int	jmp_buf[2];
#endif

#ifdef	m6805
#error	setjmp() and longjmp() cannot be implemented on the 6805
#endif

extern	int	setjmp(jmp_buf);
extern void	longjmp(jmp_buf, int);
