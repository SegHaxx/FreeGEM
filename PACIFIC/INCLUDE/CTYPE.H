#define	_U	0x01
#define	_L	0x02
#define	_N	0x04
#define	_S	0x08
#define _P	0x10
#define _C	0x20
#define	_X	0x40
#define	_B	0x80

extern	const unsigned char	_ctype_[];	/* in libc.lib */

#define	isalpha(c)	((_ctype_+1)[(unsigned char)(c)]&(_U|_L))
#define	isupper(c)	((_ctype_+1)[(unsigned char)(c)]&_U)
#define	islower(c)	((_ctype_+1)[(unsigned char)(c)]&_L)
#define	isdigit(c)	((_ctype_+1)[(unsigned char)(c)]&_N)
#define	isxdigit(c)	((_ctype_+1)[(unsigned char)(c)]&(_N|_X))
#define	isspace(c)	((_ctype_+1)[(unsigned char)(c)]&_S)
#define ispunct(c)	((_ctype_+1)[(unsigned char)(c)]&_P)
#define isalnum(c)	((_ctype_+1)[(unsigned char)(c)]&(_U|_L|_N))
#define isprint(c)	((_ctype_+1)[(unsigned char)(c)]&(_P|_U|_L|_N|_B))
#define isgraph(c)	((_ctype_+1)[(unsigned char)(c)]&(_P|_U|_L|_N))
#define iscntrl(c)	((_ctype_+1)[(unsigned char)(c)]&_C)
#define isascii(c)	(!((c)&~0x7F))
#define toascii(c)	((c)&0x7F)

extern char	toupper(int);
extern char	tolower(int);
