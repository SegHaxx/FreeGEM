/*
 *	Mouse library entry points.
 *	Colin Weaver, HI-TECH Software		October 1990
 */

#ifndef	_MOUSE_

#include	<stdlib.h>

#define	_MOUSE_	1

/*
 *	Button status bits in button status word returned by readmouse
 */

#define	LEFT_FLAG	0x0001
#define	RIGHT_FLAG	0x0002
#define	MIDDLE_FLAG	0x0004

#define	LEFT_BUTTON	0
#define	RIGHT_BUTTON	1
#define	MIDDLE_BUTTON	2

/*
 *	Cursor types (text mode)
 */

#define	SOFTWARE_CURSOR	0
#define	HARDWARE_CURSOR	1

/*
 *	Event handler mask bits
 */

#define	CURSOR_MOVED	0x0001
#define	LEFT_PRESS	0x0002
#define	LEFT_RELEASE	0x0004
#define	RIGHT_PRESS	0x0008
#define	RIGHT_RELEASE	0x0010
#define	MIDDLE_PRESS	0x0020
#define	MIDDLE_RELEASE	0x0040

#define	BUTTON_PRESS	(LEFT_PRESS | MIDDLE_PRESS | RIGHT_PRESS)
#define	BUTTON_RELEASE	(LEFT_RELEASE | MIDDLE_RELEASE | RIGHT_RELEASE)
#define	BUTTON_EVENT	(BUTTON_PRESS | BUTTON_RELEASE)

struct mousedata {
	int		y;		/* vertical position */
	int		x;		/* horizontal position */
	int		buttons;	/* button status bits */
	int		count;		/* button press count */
	int		event;		/* event bits */
	int		keymods;	/* keyboard modifier bits */
	unsigned long	timer;		/* timer value */
};

#pragma	pack(2)
struct graphic_cursor {
	unsigned short	screenmask[16];
	unsigned short	cursormask[16];
	unsigned short	hotspot_x;
	unsigned short	hotspot_y;
};
#pragma	pack(4)

struct boundsrect {
	int	top;
	int	left;
	int	bottom;
	int	right;
};

struct sensitivity {
	int	horizontal;	/* horizontal step/pixel ratio */
	int	vertical;	/* vertical step/pixel ratio */
	int	threshold;	/* ballistic threshold */
};

struct mouseinfo {
	unsigned char	minorver;
	unsigned char	majorver;
	unsigned char	irqnumber;
	unsigned char	mousetype;
};

extern int		initmouse(void);
extern void		shutdownmouse(void);
extern void		showcursor(void);
extern void		hidecursor(void);
extern void		readmouse(struct mousedata *);
extern void		setmousepos(int, int);
extern void		getpressinfo(int, struct mousedata *);
extern void		getreleaseinfo(int, struct mousedata *);
extern void		sethlimits(int, int);
extern void		setvlimits(int, int);
extern void		setgrcursor(struct graphic_cursor *);
extern void		setcursor(int, unsigned int, unsigned int, unsigned int, unsigned int);
extern void		readcounters(struct mousedata *);
extern void		eventhandler(int, void (*)(struct mousedata *));
extern void		setpixelratio(int, int);
extern void		conditionalhide(struct boundsrect *);
extern void		speedthreshold(int);
extern void		setsensitivity(struct sensitivity *);
extern void		getsensitivity(struct sensitivity *);
extern size_t		getstoragesize(void);
extern int		savemouse(void);
extern int		restoremouse(void);
extern void		getmouseinfo(struct mouseinfo *);
extern void		savestate(void *);
extern void		restorestate(void *);
extern unsigned int	keymodbits(void);

#endif

/*
 *	End of file: MOUSE.H
 */
