#include	<dos.h>
#include	<stdlib.h>
#include	"mouse.h"

/*
 *	Mouse driver interface code for standard INT 33h mouse driver.
 *	This C version replaces the earlier memory model specific assembler
 *	versions.  The 80386 assembler version (mouse_f.as) should still be
 *	used for flat model, due to the need to handle interaction between
 *	real and protected mode using DOS extender system calls.
 *
 *	Colin Weaver, HI-TECH Software		June 1992
 *
 *	This code assumes that the int86x() function returns the new values
 *	of the segment registers in the "SREGS" strcture passed.
 */

#define	DOS_INT		0x21		/* MS-DOS: int 21h */
#define	MOUSE_INT	0x33		/* Mouse driver: int 33h */

#define	ushort		unsigned short
#define	ulong		unsigned long
#define	FPTR_SEG(ptr)	(((long)(far void (*)(void))(ptr)) >> 16)
#define	GLUE_STACK_SIZE	256

far volatile unsigned long *	_TICK_COUNT_ = (far unsigned long *) 0x0040006CL;
static far unsigned short	KBSTATUS  @ 0x00400017L;

/*
 *	int	initmouse(void)
 *
 *	Initialize the mouse driver, return the number of buttons, or -1 on
 *	failure.
 */

int
initmouse(void)
{
	union REGS	reg;
	struct SREGS	seg;

	reg.x.ax = 0x3533;		/* funcion 35H, get vector: 33H */
	int86x(DOS_INT, &reg, &reg, &seg);
	if (reg.x.bx == 0 && seg.es == 0)
		return -1;		/* no mouse driver */
	reg.x.ax = 0;			/* function 0: reset mouse */
	int86(MOUSE_INT, &reg, &reg);
	if (reg.x.ax == 0)
		return -1;		/* mouse not present */
	return reg.x.bx;		/* return button count */
}

/*
 *	void	shutdownmouse(void)
 *
 *	Shut down mouse driver - dummy on PC
 */

void
shutdownmouse(void)
{
	eventhandler(0, NULL);
}

/*
 *	void	showcursor(void)
 *
 *	Increment the cursor hide/show level
 */

void
showcursor(void)
{
	union REGS	reg;

	reg.x.ax = 1;			/* function 1: show cursor */
	int86(MOUSE_INT, &reg, &reg);
}

/*
 *	void	hidecursor(void)
 *
 *	Decrement the cursor hide/show level
 */

void
hidecursor(void)
{
	union REGS	reg;

	reg.x.ax = 2;			/* function 2: hide cursor */
	int86(MOUSE_INT, &reg, &reg);
}

/*
 *	void	readmouse(struct mousedata * mouse)
 *
 *	Read mouse position and button status
 */

void
readmouse(struct mousedata * mouse)
{
	union REGS	reg;

	reg.x.ax = 3;			/* function 3: read pos'n & status */
	int86(MOUSE_INT, &reg, &reg);
	mouse->timer = *_TICK_COUNT_;
	mouse->buttons = reg.x.bx;
	mouse->x = reg.x.cx;
	mouse->y = reg.x.dx;
}

/*
 *	void	setmousepos(int row, int col)
 *
 *	Set mouse position to (row, col)
 */

void
setmousepos(int row, int col)
{
	union REGS	reg;

	reg.x.cx = col;
	reg.x.dx = row;
	reg.x.ax = 4;			/* function 4: set cursor position */
	int86(MOUSE_INT, &reg, &reg);
}

/*
 *	void	getpressinfo(int button, struct mousedata * mouse)
 *
 *	Return number of button presses for specified button
 */

void
getpressinfo(int button, struct mousedata * mouse)
{
	union REGS	reg;

	reg.x.ax = 5;			/* function 5: get button press info */
	reg.x.bx = button;
	int86(MOUSE_INT, &reg, &reg);
	mouse->timer = *_TICK_COUNT_;
	mouse->buttons = reg.x.ax;
	mouse->count = reg.x.bx;
	mouse->x = reg.x.cx;
	mouse->y = reg.x.dx;
}

/*
 *	void	getreleaseinfo(int button, struct mousedata * mouse)
 *
 *	Return number of button releases for specified button
 */

void
getreleaseinfo(int button, struct mousedata * mouse)
{
	union REGS	reg;

	reg.x.ax = 6;			/* function 6: get release info */
	reg.x.bx = button;
	int86(MOUSE_INT, &reg, &reg);
	mouse->timer = *_TICK_COUNT_;
	mouse->buttons = reg.x.ax;
	mouse->count = reg.x.bx;
	mouse->x = reg.x.cx;
	mouse->y = reg.x.dx;
}

/*
 *	void	sethlimits(int min, int max)
 *
 *	Set horizontal mouse movement bounds
 */

void
sethlimits(int min, int max)
{
	union REGS	reg;

	reg.x.ax = 7;			/* function 7: set horizontal limits */
	reg.x.cx = min;
	reg.x.dx = max;
	int86(MOUSE_INT, &reg, &reg);
}

/*
 *	void	setvlimits(int min, int max)
 *
 *	Set vertical mouse movement bounds
 */

void
setvlimits(int min, int max)
{
	union REGS	reg;

	reg.x.ax = 8;			/* function 8: set vertical limits */
	reg.x.cx = min;
	reg.x.dx = max;
	int86(MOUSE_INT, &reg, &reg);
}

/*
 *	void	setgrcursor(struct graphic_cursor * cursor)
 *
 *	Set the graphics mode cursor
 */

void
setgrcursor(struct graphic_cursor * cursor)
{
	union REGS	reg;
	struct SREGS	seg;

	reg.x.ax = 9;			/* function 9: set graphic cursor */
	reg.x.bx = cursor->hotspot_x;
	reg.x.cx = cursor->hotspot_y;
	reg.x.dx = FP_OFF(cursor);
	seg.es = FP_SEG(cursor);
	int86x(MOUSE_INT, &reg, &reg, &seg);
}

/*
 *	void	setcursor(int type,
 *			unsigned int smask_char, unsigned int smask_attr,
 *			unsigned int cmask_char, unsigned int cmask_attr);
 *
 *	Set the text mode cursor type and masks
 */

void
setcursor(int type, unsigned int smask_char, unsigned int smask_attr,
		    unsigned int cmask_char, unsigned int cmask_attr)
{
	union REGS	reg;

	reg.x.ax = 10;			/* function 10: set text cursor */
	reg.x.bx = type;
	reg.x.cx = (smask_char & 0xFF) | (smask_attr << 8);
	reg.x.dx = (cmask_char & 0xFF) | (cmask_attr << 8);
	int86(MOUSE_INT, &reg, &reg);
}

/*
 *	void	readcounters(struct mousedata * mouse)
 *
 *	Read the mouse motion counters (deltas from last read)
 */

void
readcounters(struct mousedata * mouse)
{
	union REGS	reg;

	reg.x.ax = 11;			/* function 11: read counters */
	int86(MOUSE_INT, &reg, &reg);
	mouse->x = reg.x.cx;
	mouse->y = reg.x.dx;
}

/*
 *	void	setpixelratio(int vertical, int horizontal)
 *
 *	Set mouse motion/pixel ratio
 */

void
setpixelratio(int vertical, int horizontal)
{
	union REGS	reg;

	reg.x.ax = 15;			/* function 15: set pixel ratio */
	reg.x.cx = horizontal;
	reg.x.dx = vertical;
	int86(MOUSE_INT, &reg, &reg);
}

/*
 *	void	conditionalhide(struct boundsrect * rect)
 *
 *	Conditionally hide the cursor if it is within the specified rectangle
 */

void
conditionalhide(struct boundsrect * rect)
{
	union REGS	reg;

	reg.x.ax = 16;			/* function 16: conditional hide */
	reg.x.cx = rect->left;
	reg.x.dx = rect->top;
	reg.x.si = rect->right;
	reg.x.di = rect->bottom;
	int86(MOUSE_INT, &reg, &reg);
}

/*
 *	void	speedthreshold(int threshold)
 *
 *	Set the ballistic speed threshold, beyond which exponential mouse
 *	response occurs (with supported drivers).
 */

void
speedthreshold(int threshold)
{
	union REGS	reg;

	reg.x.ax = 19;			/* function 19: set speed threshold */
	reg.x.dx = threshold;
	int86(MOUSE_INT, &reg, &reg);
}

/*
 *	void	setsensitivity(struct sensitivity * se)
 *
 *	Set the mouse sensitivity values (horizontal, vertical and
 *	ballistic threshold)
 */

void
setsensitivity(struct sensitivity * se)
{
	union REGS	reg;

	reg.x.ax = 26;			/* function 26: set sensitivity */
	reg.x.bx = se->horizontal;
	reg.x.cx = se->vertical;
	reg.x.dx = se->threshold;
	int86(MOUSE_INT, &reg, &reg);
}

/*
 *	void	getsensitivity(struct sensitivity * se)
 *
 *	Get the mouse sensitivity values (horizontal, vertical and
 *	ballistic threshold)
 */

void
getsensitivity(struct sensitivity * se)
{
	union REGS	reg;

	reg.x.ax = 27;			/* function 27: get sensitivity */
	int86(MOUSE_INT, &reg, &reg);
	se->horizontal = reg.x.bx;
	se->vertical = reg.x.cx;
	se->threshold = reg.x.dx;
}

/*
 *	size_t	getstoragesize(void)
 *
 *	Return the size of the buffer required to save the
 *	complete driver state.
 */

size_t
getstoragesize(void)
{
	union REGS	reg;

	reg.x.ax = 21;			/* function 21: driver state size */
	int86(MOUSE_INT, &reg, &reg);
	return reg.x.bx;
}

/*
 *	void	savestate(void * buffer)
 *
 *	Save the state of the mouse driver in the specified buffer
 */

void
savestate(void * buffer)
{
	union REGS	reg;
	struct SREGS	seg;

	reg.x.ax = 22;			/* function 22: save driver state */
	reg.x.dx = FP_OFF(buffer);
	seg.es = FP_SEG(buffer);
	int86x(MOUSE_INT, &reg, &reg, &seg);
}

/*
 *	void	restorestate(void * buffer)
 *
 *	Restore the mouse driver state from the specified buffer
 */

void
restorestate(void * buffer)
{
	union REGS	reg;
	struct SREGS	seg;

	reg.x.ax = 23;			/* function 23: restore driver state */
	reg.x.dx = FP_OFF(buffer);
	seg.es = FP_SEG(buffer);
	int86x(MOUSE_INT, &reg, &reg, &seg);
}

/*
 *	int	savemouse(void)
 *
 *	Save the current mouse driver state.  This function allocates the
 *	buffer required to save the driver state.
 *
 *	On exit:	return value == 1 if save was successful, 0 otherwise.
 *
 *	Two static variables are defined:
 *
 *	size_t	_m_savesize_	Size of buffer state (in case it changes)
 *	void *	_m_saveptr_	Driver state buffer pointer
 *				obtained using malloc()
 */

static size_t	_m_savesize_;
static void *	_m_saveptr_;

int
savemouse(void)
{
	if (_m_saveptr_) {			/* block already allocated */
		if (_m_savesize_ < getstoragesize()) {	/* large enough ? */
			free(_m_saveptr_);
			_m_saveptr_ = NULL;
		}
	}
	if (!_m_saveptr_) {			/* need a new block ? */
		_m_savesize_ = getstoragesize();
		_m_saveptr_ = _non_ems_alloc(_m_savesize_);
		if (!_m_saveptr_) {		/* did malloc() work ? */
			return 0;
		}
	}
	savestate(_m_saveptr_);
	return 1;
}

/*
 *	int	restoremouse(void)
 *
 *	Restore the mouse driver state.  This function uses the buffer
 *	allocated by savemouse()
 *
 *	Return code: 1 on success, 0 on failure.
 */

int
restoremouse(void)
{
	if (_m_saveptr_) {
		restorestate(_m_saveptr_);
		return 1;
	} else {
		return 0;
	}
}

/*
 *	void	getmouseinfo(struct mouseinfo * minfo)
 *
 *	Return the mouse driver version, mouse interface type and
 *	mouse IRQ number in the "minfo" structure supplied.
 *
 *	This function may not be available with some mouse drivers.
 */

void
getmouseinfo(struct mouseinfo * minfo)
{
	union REGS	reg;

	reg.x.ax = 36;
	int86(MOUSE_INT, &reg, &reg);
	minfo->majorver = reg.h.bh;
	minfo->minorver = reg.h.bl;
	minfo->mousetype = reg.h.ch;
	minfo->irqnumber = reg.h.cl;
}

/*
 *	unsigned int	keymodbits(void)
 *
 *	Return the current keyboard modifier bits
 */

unsigned int
keymodbits(void)
{
	return KBSTATUS;
}

/*
 *	void	eventhandler(int eventbits, void (*handler)(struct mousedata *))
 *
 *	Install a user specified mouse event handler function.
 *
 *	eventbits:	mask specifying which event types are to be passed
 *			to the user handler.
 *
 *	handler:	pointer to a user function which is called when
 *			an event of one of the types specified in eventbits
 *			occurs.
 *
 *	The user event handler is NOT called directly by the mouse driver.
 *	A special "glue" routine _asm_event_glue() is installed, and is
 *	responsible for setting up a sensible execution environment before
 *	passing the AX, BX, CX and DX register passed by the mouse driver
 *	into _c_event_glue_();
 */

extern void	_asm_event_glue(void);
static void	(*user_handler)(struct mousedata *);

void
eventhandler(int eventbits, void (*handler)(struct mousedata *))
{
	union REGS	reg;
	struct SREGS	seg;

	user_handler = handler;
	reg.x.ax = 12;			/* function 12: install event handler */
	reg.x.cx = (eventbits != -1)? eventbits : 0;
	reg.x.dx = FP_OFF(_asm_event_glue);
	seg.es = FPTR_SEG(_asm_event_glue);	/* function pointer segment */
	int86x(MOUSE_INT, &reg, &reg, &seg);
}

/*
 *	void	_c_event_glue_(ushort DX, ushort AX, ushort BX, ushort CX)
 *
 *	Event handler code: called from the _asm_event_glue() assembler routine.
 *	As much event processing as possible is performed using C code, the
 *	_asm_event_glue() routine is used to set up a sensible environment
 *	before re-entering the C code.
 *
 *	When called from assembler code, _c_event_glue_() takes its arguments
 *	as follows:
 *
 *	CX	on stack
 *	BX	on stack, pushed after CX
 *	AX	in ax register
 *	DX	in dx register
 *
 *	*** WARNING *** these argument conventions are HI-TECH C specific
 */

void
_c_event_glue_(ushort DX, ushort AX, ushort BX, ushort CX)
{
	static struct mousedata	mouse;

	if (user_handler) {
		mouse.event = AX;
		mouse.buttons = BX;
		mouse.x = CX;
		mouse.y = DX;
		mouse.count = 0;
		mouse.keymods = KBSTATUS;
		mouse.timer = *_TICK_COUNT_;
		(*user_handler)(&mouse);
	}
}

/*
 *	void	_asm_event_glue(void);
 *
 *	Event handler code, called interrupt style from the mouse driver.
 *	This routine is responsible for passing the information in the CPU
 *	registers into _c_event_glue_() routine, which in turn calls the user
 *	supplied handler with the structure as an argument.
 *
 *	For obvious reasons, this function is written in assembly language
 *	and is very compiler specific.
 */

#asm
		.psect	bss,class=BSS
		.align	2
;
;	Scratch pad values used by event glue code
;
		.globl	__Lbss
__old_ss:	.blkb	2
__old_sp:	.blkb	2
__old_ds:	.blkb	2
__old_es:	.blkb	2
#if	LARGE_DATA
		.psect	lbss,local,class=LBSS,reloc=16,size=65535
#endif
		.blkb	GLUE_STACK_SIZE
__glue_stack:
;
#if	SMALL_CODE
		.psect	_TEXT,class=CODE,reloc=16,size=65535
#else
		.psect	ltext,local,class=CODE,reloc=16,size=65535
#endif
		.align	16
__asm_event_glue:
		PUSHF				;keep current flag state
		CLI				;ints off for the duration
		CLD				;direction forwards
		PUSH	AX			;keep event bits
		PUSH	DS			;and driver DS
		MOV	AX,#seg(__Lbss)
		MOV	DS,AX			;setup user DS
		POP	__old_ds
		MOV	__old_es,ES
		MOV	__old_ss,SS
		MOV	__old_sp,SP		;keep driver stack
		MOV	ES,__old_ss		;ES indexes old stack
		MOV	AX,#seg(__glue_stack)
		MOV	SS,AX
		MOV	SP,#__glue_stack	;setup our own stack
		PUSH	CX
		PUSH	BX
		MOV	BX,__old_sp
		MOV	AX,ES:[BX]		;get driver AX value
#if	SMALL_CODE
		CALL	__c_event_glue_
#else
		CALLF	__c_event_glue_
#endif
		MOV	SS,__old_ss		;get driver SS
		MOV	SP,__old_sp		;get driver SP
		MOV	ES,__old_es		;get driver ES
		MOV	DS,__old_ds		;get driver DS
		POP	AX
		POPF
		RETF
#endasm

/*
 *	End of file: MOUSE.C
 */
