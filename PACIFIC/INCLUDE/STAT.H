#ifndef	_STATDEF
#define	_STATDEF
#if	ATDOS
struct stat
{
	unsigned long	st_ino;		/* unique file number */
	unsigned short	st_dev;		/* id of device containing directory entry */
	unsigned short	st_nlink;	/* # of links */
	unsigned short	st_mode;	/* flags */
	unsigned short	st_uid;		/* owner uid */
	long		st_atime;	/* access time */
	long		st_mtime;	/* modification time */
	long		st_ctime;	/* time of last file status 'change' */
	long		st_size;	/* file size in bytes */
};

#define	MAXNAMLEN	255

extern int	readdir(int, struct dirbuf *);

/* Flag bits in st_mode */

#define	S_IFMT		0x638	/* type bits */
#define		S_IFDIR	0x400	/* is a directory */
#define		S_IFREG	0x200	/* is a regular file */
#define		S_IFBLK	0x600	/* is a block device */
#define		S_IFCHR	0x608	/* is a character device */
#define	S_IREAD		0x100	/* file can be read */
#define	S_IWRITE	0x080	/* file can be written */
#define	S_IEXEC		0x040	/* file can be executed */
#define	S_HIDDEN	0x1000	/* file is hidden */
#define	S_SYSTEM	0x2000	/* file is marked system */
#define	S_ARCHIVE	0x4000	/* file has been written to */

#define	S_IRWXU	00700		/* read, write, execute: owner */
#define	S_IRUSR	00400		/* read permission: owner */
#define	S_IWUSR	00200		/* write permission: owner */
#define	S_IXUSR	00100		/* execute permission: owner */
#define	S_IRWXO	00007		/* read, write, execute: other */
#define	S_IROTH	00004		/* read permission: other */
#define	S_IWOTH	00002		/* write permission: other */
#define	S_IXOTH	00001		/* execute permission: other */

#else	/* ATDOS */

struct stat
{
	unsigned long	st_ino;
	unsigned short	st_dev;
	unsigned short	st_mode;	/* flags */
	long		st_atime;	/* access time */
	long		st_mtime;	/* modification time */
	long		st_size;	/* file size in bytes */
};

#define	MAXNAMLEN	12

/* Flag bits in st_mode */

#define	S_IFMT		0x600	/* type bits */
#define		S_IFDIR	0x400	/* is a directory */
#define		S_IFREG	0x200	/* is a regular file */
#define	S_IREAD		0400	/* file can be read */
#define	S_IWRITE	0200	/* file can be written */
#define	S_IEXEC		0100	/* file can be executed */
#define	S_HIDDEN	0x1000	/* file is hidden */
#define	S_SYSTEM	0x2000	/* file is marked system */
#define	S_ARCHIVE	0x4000	/* file has been written to */

#endif	/* ATDOS */

struct dirbuf
{
	struct stat	di_stat;		/* info on the file */
	char		di_name[MAXNAMLEN+1];	/* the file name */
};
#endif	_STATDEF

extern int		stat(char *, struct stat *);
extern struct dirbuf *	ffirst(char *);
extern struct dirbuf *	fnext(void);
