#include	<stdio.h>
#include	<conio.h>
#include	<stdlib.h>
#include	<intrpt.h>

/*
 *	program to set key repeat rate on PC/AT type keyboard
 *	C.Weaver, Oct '91
 */

port unsigned char	kb_data @ 0x60;
port unsigned char	kb_status @ 0x64;

int
sendbyte(unsigned char data)
{
	int	rcode;
	register int	retry;
	register int	loop;

	rcode = 1;
	di();
	for (retry = 0; retry != 8; retry++) {
		for (loop = 0; loop != 0x7FFF; loop++)
			if (!(kb_status & 2))
				break;
		kb_data = data;
		for (loop = 0; loop != 0x7FFF; loop++)
			if (kb_status & 1)
				break;
		if (kb_data == 0xFA)
			goto done;
	}
	rcode = 0;
done:
	ei();
	return rcode;
}

int
kbdsend(unsigned char data)
{
	if (!sendbyte(0xF3))
		return 0;
	return sendbyte(data);
}

void
help(void)
{
	cputs("keyrate: usage: keyrate delay reprate\n\n"
	"delay (before repeat) values:\n"
	"1 = 1/4 sec, 2 = 1/2 sec, 3 = 3/4 sec, 4 = 1 sec\n\n"
	"reprate (repeats per second) values:\n"
	"2, 3, 4, 5, 6, 8, 10, 12, 15, 16, 20, 24, 30\n");
	exit(1);
}

main(argc, argv)
int	argc;
char **	argv;
{
	int		delay, reprate;

	if (argc != 3)
		help();
	delay = atoi(argv[1]);
	if (delay < 1 || delay > 4)
		help();
	reprate = atoi(argv[2]);
	switch (reprate) {
	case 2:
		reprate = 0x1F;
		break;
	case 3:
		reprate = 0x1A;
		break;
	case 4:
		reprate = 0x17;
		break;
	case 5:
		reprate = 0x14;
		break;
	case 6:
		reprate = 0x12;
		break;
	case 8:
		reprate = 0x0F;
		break;
	case 10:
		reprate = 0x0C;
		break;
	case 12:
		reprate = 0x0A;
		break;
	case 15:
		reprate = 0x08;
		break;
	case 16:
		reprate = 0x03;
		break;
	case 20:
		reprate = 0x04;
		break;
	case 24:
		reprate = 0x02;
		break;
	case 30:
		reprate = 0x00;
		break;
	default:
		help();
	}
	if (kbdsend(((delay - 1) << 5) | reprate))
		cputs("keyrate: keyboard repeat rate set\n");
	else
		cputs("keyrate: couldn't talk to keyboard\n");
}
