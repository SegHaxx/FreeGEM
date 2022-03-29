/*
 *	Pacific C mouse driver example code.
 *	Enables the mouse, then displays mouse position and
 *	button status every time the user presses or releases
 *	a button, until the user presses a key.
 */

#include	<stdio.h>
#include	<conio.h>
#include	<stdlib.h>
#include	"mouse.h"

volatile int	buttons, x, y, got_event;

/*
 *	Mouse event handler.  This is directly called by the
 *	mouse driver when a mouse event happens.  Note that
 *	got_event, buttons, x and y need to be volatile because
 *	they are accessed asynchronously by an event handler and
 *	the main program.
 *
 *	WARNING!  Do not try doing complex things like printf()
 *	calls in a handler installed by event_handler().
 */

void
mouse_event(struct mousedata * mouse)
{
	if (got_event == 0) {
		buttons = mouse->buttons;
		x = mouse->x;
		y = mouse->y;
		got_event = 1;
	}
}

/*
 *	Main program - installs mouse_event() to handle
 *	mouse events asynchronously, then polls the
 *	flags set by mouse_event() when something happens.
 *	This allows the user code to do other things while
 *	waiting for the mouse state to change.
 *
 *	Try making the first parameter to eventhandler()
 *	CURSOR_MOVED | BUTTON_PRESS | BUTTON_RELEASE
 */

main()
{
	buttons = initmouse();	/* try to initialize mouse */
	if (buttons == -1) {
		printf("Mouse not present\n");
		exit(0);
	} else {
		printf("%d button mouse found\n", buttons);
	}
	showcursor();		/* turn on mouse cursor */
	got_event = 0;
	eventhandler(BUTTON_PRESS | BUTTON_RELEASE, mouse_event);
	while (!kbhit()) {
		if (got_event != 0) {
			hidecursor();
			printf("X = %3d  Y = %3d  %s  %s  %s\n", x, y,
				(buttons & LEFT_FLAG)?   "left"   : "",
				(buttons & MIDDLE_FLAG)? "middle" : "",
				(buttons & RIGHT_FLAG)?  "right"  : "");
			showcursor();
			got_event = 0;
		}
	}
	getch();		/* clear keystroke */
	hidecursor();		/* turn of mouse cursor */
	shutdownmouse();
}
