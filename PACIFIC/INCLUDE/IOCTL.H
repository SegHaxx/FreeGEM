/*
 *	Header info for the DOS ioctl() call
 *
 *	ioctl(file, func, val)
 *	int file, func, val;
 *
 */

extern int	ioctl(int, int, ...);

#define	GETINFO	0		/* return device flag word */
#define	SETINFO	1		/* set device info */
#define	GETISTS	6		/* get input status */
#define	GETOSTS	7		/* get output status */

/*
 *	Flag bits returned from GETINFO
 */

#define	ISCIN	1		/* device is console input */
#define	ISCOUT	2		/* device is console output */
#define	ISNUL	4		/* device is the black hole */
#define	ISCLK	8		/* tick tock */
#define	RAW	0x20		/* not cooked, dummy! */
#define	ATEOF	0x40		/* at EOF */
#define	ISDEV	0x80		/* this is a device, not a file */
#define	CTRL	0x4000		/* can process control strings, whatever they are */

