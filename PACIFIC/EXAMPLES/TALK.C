/*
 *	Simple talk program which will talk to whatever is on the end
 *	of a standard PC serial port.
 *
 *	Usage:
 *		talk [-s19200] [comN]
 *
 *	Defaults:	19200 baud, COM1
 *
 *	Example, to talk to COM2 at 4800 baud:
 *
 *	talk -s4800 com2
 *
 *	To compile:
 *
 *	c -O -Zg talk.c
 *
 */

#include	<ctype.h>
#include	<conio.h>
#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>
#include	<signal.h>
#include	<ioctl.h>
#include	<intrpt.h>

#define BOOL	unsigned char
#define FALSE	(BOOL)0
#define TRUE	(BOOL)1

#define MENU		0x1C
#define CONTINUE	0
#define FINISH		1

static BOOL	debug = FALSE;
static BOOL	printable[32] = {0,0,0,0,0,0,0,1,1,1,1,0,0,1,0,0,
				 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

/*	Routines to access an IBM serial port	*/

#define	PORT1	((port unsigned char *)0x3F8)	/* COM1 */
#define	PORT2	((port unsigned char *)0x2F8)	/* COM2 */

#define	IRQ4	(far isr *)0x30		/* for COM1 */
#define	IRQ3	(far isr *)0x2C		/* for COM2 */

#define	MSK4	0x10
#define	MSK3	0x08

#define	ICR	((port unsigned char *)0x20)	/* interrupt control register */
#define	IMR	((port unsigned char *)0x21)	/* interrupt mask register */
#define	EOI	0x20				/* end of interrupt command */

static port unsigned char *	PORT;

static struct
{
	unsigned	rate;
	unsigned char	bits;
}	baurates[] =
{
	38400, 3,
	19200, 6,
	9600, 12,
	4800, 24,
	2400, 48,
	1200, 96
};

static char		inbuf[256];
static unsigned char	iptr, optr, tmp;
static BOOL		kchar;

/*
 *	Prototypes for all functions in talk.c
 */

static unsigned char	gbyte(unsigned char);
static unsigned char	menu(void);
static void		cflush(void);
static void		cominit(char *, unsigned short);
static void interrupt	service(void);
static void		sbyte(unsigned char);
static void		sendfile(void);
static void		sinkline(void);
static void		terminal(void);

static void interrupt
service(void)
{
	inbuf[iptr] = *PORT;
	tmp = iptr+1;
	if(tmp != optr)
		iptr = tmp;
	*ICR = EOI;
}

void
disint(void)
{
	PORT[1] = 0;
	*IMR = *IMR | (PORT == PORT1 ? MSK4 : MSK3);
}

static void
cominit(comname, baud)
register char *	comname;
unsigned short	baud;
{
	register unsigned char	i;

	if(strcmp(comname, "com2") == 0 || strcmp(comname, "COM2") == 0)
		PORT = PORT2;
	else
		PORT = PORT1;
	for(i = 0 ; baurates[i].rate != baud ; i++)
		if(i == sizeof baurates/sizeof baurates[0]) {
			i = 1;	/* default 19200 */
			break;
		}
	set_vector(PORT == PORT1 ? IRQ4 : IRQ3, service);
	PORT[3] = 0x80;	/* enable divisor latches */
	PORT[0] = baurates[i].bits;
	PORT[1] = baurates[i].bits >> 8;
	PORT[3] = 3;	/* 8 bits, 1 stop, no parity */
	PORT[4] = 0xB;	/* set DTR and RTS , enable interrupts */
	i = *PORT;		/* clear rx buffer */
	PORT[1] = 1;	/* enable receive interrupts */
	*IMR = *IMR & ~(PORT == PORT1 ? MSK4 : MSK3);
	atexit(disint);
}

static void
sbyte(c)
unsigned char	c;
{
	PORT[1] = 1;
	*IMR = *IMR & ~(PORT == PORT1 ? MSK4 : MSK3);
	while((PORT[5] & 0x20) == 0)
		continue;
	*PORT = c;
}

static unsigned char
gbyte(waitchar)
BOOL	waitchar;
{
	register unsigned char	c;

	kchar = FALSE;
	PORT[1] = 1;
	*IMR = *IMR & ~(PORT == PORT1 ? MSK4 : MSK3);
	if (waitchar)
		while(iptr == optr)
			if(kbhit()) {
				kchar = TRUE;
				return getch();
			}
	if (iptr != optr) {
		di();
		c = inbuf[optr++];
		ei();
		return c;
	} else
		return 0;
}

static void
cflush()
{
	iptr = optr;
}

static void
sinkline()
{
	while ((gbyte(TRUE) != 13) && !kchar) ;
}

static void
sendfile()
{
	register FILE	*hexfile;
	register char	*bufptr;
	char		buf[256];
	unsigned char	abort;

	printf("\nEnter pathname:");
	gets(buf);
	if (hexfile = fopen(buf, "r")) {
		printf("Sending file...\n");
		abort = 0;
		while (fgets(buf, sizeof buf, hexfile)) {
			for (bufptr = buf; *bufptr && !abort; bufptr++) {
				if (*bufptr >= 32 || *bufptr == 13)
					sbyte(*bufptr);
				abort = gbyte(FALSE);
			}
			if (abort)
				break;
		}
		if (abort)
			sinkline();
		printf("Finished\n");
		fclose(hexfile);
	} else
		printf("File not found\n");
}

static unsigned char
menu()
{
	char	option;

	printf("\nTALK MENU\nD - turn debug display %s\nS - send a file\nX - exit to DOS\n\nAny other key re-connects\n\nEnter option - ", debug ? "OFF" : "ON");
	fflush(stdout);
	putch(option = toupper(getch()));
	switch(option) {
		case 'D':
			debug = !debug;
			break;
		case 'S':
			sendfile();
			break;
		case 'X':
			return FINISH;
	}
	putch('\n');
	return CONTINUE;
}

static void
terminal()
{
	unsigned char	c;

	signal(SIGINT, SIG_IGN);
	for(;;) {
		c = gbyte(TRUE);
		if (kchar) {
			if (c != MENU)
				sbyte(c);
			else if (menu() == FINISH)
				return;
		} else {
			if (c < 32) {
				if (debug) {
					printf("[%02X]", c);
					fflush(stdout);
				} else if (printable[c])
					putch(c);
			} else
				putch(c);
		}
	}
}

main(argc, argv)
int	argc;
char	**argv;
{
	char	*comname = "COM1";
	char	*speed = "19200";

	setvbuf(stdout, 0, _IOLBF, 0);
	if (argc > 1) {
		if (argv[1][0] == '-' && tolower(argv[1][1]) == 's') {
			speed = argv[1] + 2;
			++argv;
			--argc;
		}
		comname = argv[1];
	}
	cominit(comname, atoi(speed));
	cflush();
	printf("TALK VERSION 1.00\nEnter control-\\ for TALK menu\n");
	terminal();
	disint();
	printf("\nFinished\n");
	exit(0);
}
