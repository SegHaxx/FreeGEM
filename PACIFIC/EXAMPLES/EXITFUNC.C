#include	<stdlib.h>
#include	<conio.h>

static void
exitfunc(void)
{
	cputs("\n\n*** THIS IS EXITFUNC ***\n\n");
}

main()
{
	atexit(exitfunc);
	cputs("exitfunc() will be automatically executed when this program terminates.\n");
	cputs("The atexit() function is specified in the ANSI standard, and is guaranteed\n");
	cputs("to work with any conforming version of C.  You can use atexit() to install\n");
	cputs("a cleanup function which will make sure your application exits properly\n");
	cputs("even in abnormal situations.  For example, if you have your own interrupt\n");
	cputs("handler on a vector, you can use atexit() to make sure the vector is restored\n");
	cputs("and the interrupt is disabled when your program exits.\n");
	cputs("\nPress any key to exit: ");
	getch();
}
