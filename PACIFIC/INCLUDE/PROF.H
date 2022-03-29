/*
 *	Defines for execution profiling.
 */

/*	The mon.out header */

struct prof_hdr
{
	short		pr_nfunc;	/* no. of functions profiled */
	short		pr_shcnt;	/* shift count for profile map */
	unsigned	pr_ltext;	/* low address of profiling range */
	unsigned	pr_htext;	/* hi address of profiling range */
	short		pr_trate;	/* timer rate (hz) * 100 */
};

/* each function entry is like this */

struct prof_func
{
	unsigned	pr_addr;	/* function address */
	long		pr_cnt;		/* no. of times executed */
};

/* type for tick counts */

typedef unsigned short	prof_tick;

/* standard profile output file name */

#define	PROF_NAME	"mon.out"
#define	MAX_FUNC	600		/* max no. of funcs that eprof handles */
