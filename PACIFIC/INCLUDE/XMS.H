/*
 *	$Header: /home1/colin/xms/RCS/xms.h,v 1.15 94/08/12 11:31:24 colin Exp $
 *
 *	HI-TECH/Pacific C XMS Routines
 *
 *	Written by Colin Weaver, July 1994
 *	Copyright (C) 1994, HI-TECH Software
 *	All Rights Reserved
 *
 *	This software remains the property of HI-TECH SOFTWARE and is
 *	supplied under licence only. The use of this software is
 *	permitted under the terms of that licence only. Copying of
 *	this software except for the purpose of making backup or
 *	working copies for the use of the licensee on a single
 *	processor is prohibited.
 *
 *	$Log:	xms.h,v $
 * Revision 1.15  94/08/12  11:31:24  colin
 * Added XVOIDPTR type, changed XMEMALLOC, XMEMFREE, XMEMGET and XMEMPUT
 * to use "glue" functions instead of in-line code generation.
 * 
 * Revision 1.14  94/08/12  10:23:46  colin
 * Added XNULL macro for XMEMPTR null test.
 * 
 * Revision 1.13  94/08/11  16:11:36  colin
 * Added XMEMPUT and XMEMGET macros for transparent XMEMPTR read/write
 * 
 * Revision 1.12  94/08/11  10:26:32  colin
 * Added XMEMALLOC and XMEMFREE macros for allocation and de-allocation
 * of pointers declared using the XMEMPTR macro.
 * 
 * Revision 1.11  94/08/10  16:58:42  colin
 * Added XMEMPTR macro for parameterised declaration of types and
 * variables which are a union of an XMS pointer and a conventional
 * memory pointer.
 * 
 * Revision 1.10  94/08/05  10:21:11  colin
 * Upgraded _xms_sbrk(), _xms_malloc(), etc., to use 32 bit block sizes.
 * 
 * Revision 1.9  94/08/03  14:11:30  colin
 * In order to avoid name confusion, renamed low level routines as follows:
 *   _xms_free()	   -->	_xms_dispose()
 *   _xms_realloc()	   -->	_xms_resize()
 *   _xms_realloc_bytes()  -->	_xms_resize_bytes()
 *   _xms_calloc()	   -->	_xms_calloc()
 * Added "high level" XMS routines, equivalent to the <stdlib.h> routines:
 * _xms_sbrk(), _xms_malloc(), _xms_free(), _xms_realloc(), _xms_calloc(),
 * _xms_put() and _xms_get().  New type _XMS_POINTER which is used to encode
 * an XMS handle/offset pair, used by the new XMS routines.
 * 
 * Revision 1.8  94/08/01  16:55:18  colin
 * Added _xms_memmove() function (XMS to XMS block copy)
 * 
 * Revision 1.7  94/07/29  16:53:22  colin
 * New routines _xms_calloc() and _xms_memset().
 * 
 * Revision 1.6  94/07/26  13:00:49  colin
 * Added prototypes for "high level" XMS routines: _xms_flush(),
 * _xms_write() and _xms_read().
 * 
 * Revision 1.5  94/07/22  16:52:30  colin
 * Changed _xms_realloc() and _xms_realloc_bytes() to return an _XMS_HANDLE.
 * 
 * Revision 1.4  94/07/21  12:06:13  colin
 * Added "used" field to _XMS_FREE structure.
 * 
 * Revision 1.3  94/07/20  14:37:39  colin
 * Added prototypes for byte count oriented allocation routines
 * _xms_alloc_bytes() and _xms_realloc_bytes().
 * 
 * Revision 1.2  94/07/19  16:04:12  colin
 * Changed struct _XMS_VERSION to split revision number into
 * major and minor revisions.  Renamed version number fields.
 * 
 * Revision 1.1  94/07/19  12:52:49  colin
 * Initial revision
 * 
 */

#if	!defined(_XMS_H_)
#define	_XMS_H_	1

#include	<stdlib.h>
#include	<string.h>

/*
 *	typedef _XMS_HANDLE:	XMS Block Handle
 *	typedef _XMS_SIZE_T:	XMS Block Size Type
 */

typedef unsigned int	_XMS_HANDLE;
typedef unsigned int	_XMS_SIZE_T;

/*
 *	struct _XMS_VERSION:	XMS Version Information
 */

struct _XMS_VERSION {
	unsigned char	ver_major;	/* Major version number */
	unsigned char	ver_minor;	/* Minor version number */
	unsigned char	rev_major;	/* Major revision number */
	unsigned char	rev_minor;	/* Minor revision number */
	unsigned int	hma_exists;	/* HMA existence flag */
};

/*
 *	struct _XMS_FREE:	XMS Free Memory
 */

struct _XMS_FREE {
	_XMS_SIZE_T	maxblock;	/* largest free block (Kb) */
	_XMS_SIZE_T	total;		/* total free XMS (Kb) */
	_XMS_SIZE_T	used;		/* XMS used */
};

/*
 *	struct _XMS_MOVE:	XMS Move Parameter Block
 *
 *	The XMS Move Parameter Block is used to control moves between
 *	extended and conventional memory.  The _XMS_MOVE block can also
 *	control moves entirely within extended or conventional memory.
 *
 *	The fields within an _XMS_MOVE block are used as follows:
 *
 *	"count"		number of bytes to transfer
 *
 *	"src_handle"	XMS handle of transfer source, or 0 if the
 *			source is in conventional memory.
 *	"src.ptr"	only if "src_handle == 0", pointer to source
 *			block in conventional memory
 *	"src.offset"	only if "src_handle != 0", offset of source
 *			within the XMS block described by "src_handle".
 *
 *	"dst_handle"	XMS handle of transfer destination, or 0 if the
 *			destination is in conventional memory.
 *	"dst.ptr"	only if "dst_handle == 0", pointer to destination
 *			block in conventional memory
 *	"dst.offset"	only if "dst_handle != 0", offset of destination
 *			within the XMS block described by "dst_handle".
 *
 *	E.g. to store a string to XMS:
 *
 *	struct _XMS_MOVE	move;
 *	_XMS_HANDLE		handle;
 *	char			string[256];
 *
 *	strcpy(string, "TEST DATA!\n");
 *	move.count = sizeof(string);
 *	move.src_handle = 0;
 *	move.src.ptr = string;
 *	move.dst_handle = handle;
 *	move.dst.offset = 0;
 *	if (!_xms_move(&move))
 *		printf("XMS MOVE FAILED!\n");
 */

union _ptr_ofs {
	far void *	ptr;
	unsigned long	offset;
};

struct _XMS_MOVE {
	unsigned long	count;		/* transfer byte count */
	_XMS_HANDLE	src_handle;	/* source XMS handle */
	union _ptr_ofs	src;		/* source offset or address */
	_XMS_HANDLE	dst_handle;	/* destination XMS handle */
	union _ptr_ofs	dst;		/* destination offset or address */
};

/*
 *	struct _XMS_INFO:	EMB Handle Information
 */

struct _XMS_INFO {
	unsigned char	lock_count;	/* lock count of block */
	unsigned char	free_handles;	/* number of free handles */
};

/*
 *	typedef _XMS_DRIVER:	Pointer to XMS Driver
 */

typedef	far void *	_XMS_DRIVER;

/*
 *	_xms_errno:	XMS error flag.
 */

extern unsigned char	_xms_errno;		/* error number from last call */
extern unsigned char	_xms_driver_realloc;	/* use driver realloc call only */

/*
 *	_xms_error codes.  The codes below are the list of XMS error
 *	codes as documented for XMS 2.0.  Not all of these codes may
 *	be returned by the C functions implemented.  It is possible
 *	that newer XMS implementations may return codes which are not
 *	defined in this list.
 *
 *	First code group is for HI-TECH/Pacific C specific:
 */

#define	XM_NO_CACHE	0xFC	/* XMS cache couldn't be created */
#define	XM_MOVE_FAILED	0xFD	/* MOVE in realloc failed */
#define	XM_BAD_ARG	0xFE	/* BAD argument to library function */
#define	XM_NO_INIT	0xFF	/* XMS routines not initialized */

/*
 *	The codes below are standard XMS codes
 */

#define	XM_NO_ERROR	0x00	/* no XMS error */
#define	XM_NO_FUNC	0x80	/* function not implemented */
#define	XM_VDISK	0x81	/* VDISK device detected */
#define	XM_ERR_A20	0x82	/* A20 error occurred */
#define	XM_NO_HMA	0x90	/* HMA does not exist */
#define	XM_HMA_USED	0x91	/* HMA already in use */
#define	XM_NO_HMA_ALLOC	0x93	/* HMA was not allocated */
#define	XM_A20_ENABLED	0x94	/* A20 line still enabled */
#define	XM_NO_MEM	0xA0	/* all extended memory is allocated */
#define	XM_NO_HANDLES	0xA1	/* all extended memory handles are in use */
#define	XM_BAD_HANDLE	0xA2	/* handle is invalid */
#define	XM_BAD_SRC	0xA3	/* invalid source handle */
#define	XM_BAD_SRC_OFS	0xA4	/* invalid source offset */
#define	XM_BAD_DEST	0xA5	/* invalid destination handle */
#define	XM_BAD_DEST_OFS	0xA6	/* invalid destination offset */
#define	XM_BAD_LENGTH	0xA7	/* length is invalid */
#define	XM_BAD_OVERLAP	0xA8	/* move has invalid overlap */
#define	XM_PARITY_ERROR	0xA9	/* parity error occurred */
#define	XM_NOT_LOCKED	0xAA	/* block is not locked */
#define	XM_LOCKED	0xAB	/* block is locked */
#define	XM_LOCK_COUNT	0xAC	/* block's lock count overflowed */
#define	XM_LOCK_FAILED	0xAD	/* lock failed */
#define	XM_SMALLER_UMB	0xB0	/* a smaller UMB is available */
#define	XM_NO_UMBS	0xB1	/* no UMBs are available */
#define	XM_UMB_INVALID	0xB2	/* UMB segment number is invalid */

/*
 *	The functions below are the low level driver functions, these
 *	all translate to one or more INT 2FH or XMS Driver calls.  No
 *	cacheing is performed by these routines, however the _xms_write
 *	and _xms_realloc routines may invalidate the cache.
 */

extern int		_xms_installed(void);
extern _XMS_DRIVER	_xms_driver(void);
extern int		_xms_version(struct _XMS_VERSION * vers);
extern int		_xms_memavail(struct _XMS_FREE * mem);
extern _XMS_HANDLE	_xms_alloc(_XMS_SIZE_T size);
extern _XMS_HANDLE	_xms_alloc_bytes(unsigned long size);
extern int		_xms_dispose(_XMS_HANDLE handle);
extern int		_xms_move(struct _XMS_MOVE * move);
extern unsigned long	_xms_lock(_XMS_HANDLE handle);
extern int		_xms_unlock(_XMS_HANDLE handle);
extern int		_xms_info(_XMS_HANDLE handle, struct _XMS_INFO * info);
extern _XMS_HANDLE	_xms_resize(_XMS_HANDLE handle, _XMS_SIZE_T size);
extern _XMS_HANDLE	_xms_resize_bytes(_XMS_HANDLE handle, unsigned long size);

/*
 *	The next group of functions implement a higher level interface
 *	to XMS which performs cacheing of memory reads and writes.  These
 *	routines also hide many of the "dirty" aspects of XMS such as the
 *	inability of most drivers to perform odd length transfers.
 */

extern void		_xms_flush(_XMS_HANDLE handle);
extern int		_xms_write(_XMS_HANDLE dst, unsigned int size,
				unsigned long ofs, far void * src);
extern int		_xms_read(_XMS_HANDLE src, unsigned int size,
				unsigned long ofs, far void * dst);
extern _XMS_HANDLE	_xms_zalloc(unsigned int n_elem, unsigned int size);
extern int		_xms_memset(_XMS_HANDLE dst, unsigned long ofs,
				int ch, unsigned long size);
extern int		_xms_memmove(_XMS_HANDLE dst, unsigned long d_ofs,
				_XMS_HANDLE src, unsigned long s_ofs, unsigned long size);

/*
 *	The final group of functions implement the highest level XMS support.
 *	These routines are XMS equivalents of the standard C <stdlib.h> memory
 *	allocation functions, as follows:
 *
 *	XMS function		<stdlib.h> equivalent
 *	---------------------------------------------
 *	_xms_sbrk()		sbrk()			
 *	_xms_malloc()		malloc()
 *	_xms_free()		free()
 *	_xms_realloc()		realloc()
 *	_xms_calloc()		calloc()
 *
 *	These routines allocate and manage memory within larger XMS blocks.
 *	If possible, all allocated memory will reside in a single XMS block
 *	which will be resized as necessary.  These routines should be used
 *	in preference to the "low level" routines defined earlier because
 *	XMS handles are a scarce resource.
 *
 *	All blocks handled by these routines are described by the _XMS_POINTER
 *	type, which is defined as follows:
 */

typedef struct {
	_XMS_HANDLE	handle;		/* handle of XMS block */
	unsigned long	offset;		/* offset of XMS block */
}	_XMS_POINTER;

/*
 *	The "handle" and "offset" fields of an _XMS_POINTER may be passed
 *	to the XMS cache routines above, or even copied to an _XMS_MOVE
 *	block for a low level XMS move.  For example:
 *
 *	_XMS_POINTER	ptr;
 *	char		string[128];
 *	int		len;
 *
 *	len = strlen(string);
 *	if (_xms_malloc(len, &ptr))
 *		_xms_write(ptr.handle, len + 1, ptr.offset, string);
 *
 *	The _xms_get() and _xms_put() routines provide an interface to
 *	_xms_read() and _xms_write() which handles _XMS_POINTERS directly.
 *
 *	The XMS memory allocation routines below all take a pointer to
 *	an _XMS_POINTER block which will be modified as necessary.  Reference
 *	passing of _XMS_POINTER structures is used due to the poor efficiency
 *	of structure passing with some C implementations.  For all functions,
 *	the address of an _XMS_POINTER is passed as the last argument.  All of
 *	these functions return an int which will be 1 on success, 0 on failure.
 *	"_xms_errno" will also be conditioned.
 *
 *	The XMS routines will remember the size of each XMS block, down to the
 *	granularity of the allocation schemes.  It is the responsibility of the
 *	application code to remember the exact size of XMS objects and to avoid
 *	overwriting the bounds of XMS blocks.
 */

extern int	_xms_sbrk(unsigned long size, _XMS_POINTER * ptr);
extern int	_xms_malloc(unsigned long size, _XMS_POINTER * ptr);
extern int	_xms_free(_XMS_POINTER * ptr);
extern int	_xms_realloc(unsigned long size, _XMS_POINTER * ptr);
extern int	_xms_calloc(unsigned int n_elem, unsigned int size, _XMS_POINTER * ptr);
extern int	_xms_get(void * dst, unsigned int size, _XMS_POINTER * src);
extern int	_xms_put(void * src, unsigned int size, _XMS_POINTER * dst);

/*
 *	The macros below provide a mechanism for declaring and manipulating
 *	objects which may reside in either XMS or conventional memory without
 *	knowing which memory type is being used by the pointer.  These macros
 *	are as follows:
 *	
 *	XMEMPTR:	may be used to declare a union which consists of an
 *			_XMS_POINTER and a conventional pointer to a specified
 *			type.  The name of the new type, and the type of the
 *			conventional memory pointer are passed as parameters.
 *
 *	XMEMALLOC:	is used to allocate storage for a pointer declared using
 *			XMEMPTR.  sizeof(ptrtype) bytes of XMS or conventional
 *			memory will be allocated and assigned to either the "xms"
 *			or "mem.ptr" field of the union.  XMS is allocated first,
 *			then conventional memory if _xms_malloc() fails.
 *
 *	XMEMFREE:	is used to free storage associated with a pointer declared
 *			using XMEMPTR.  Either _xms_free() or free() will be used
 *			to deallocate the memory associated with the pointer.
 *
 *	XMEMPUT:	is used to write to the memory associated with a pointer
 *			declared using XMEMPTR.  Either memcpy() or _xms_put() will
 *			be used to write to the memory owned by the pointer.
 *
 *	XMEMGET:	is used to read from the memory associated with a pointer
 *			declared using XMEMPTR.  Either memcpy() or _xms_get() will
 *			be used to read from the memory owned by the pointer.
 *
 *	XNULL:		may be used in an expression to test whether an XMEMPTR is
 *			NULL. e.g. if (XNULL(ptr))
 */

#define	XMEMPTR(ptrtype)	union { \
	_XMS_POINTER	xms; \
	struct { \
		_XMS_HANDLE	handle; \
		ptrtype *	ptr; \
	} mem; \
}

typedef XMEMPTR(void)	XVOIDPTR;	/* XMS void pointer type */

#define	XMEMALLOC(size, p)	__xmemalloc__(size, (XVOIDPTR *) &(p))
#define	XMEMFREE(p)		__xmemfree__((XVOIDPTR *) &(p))
#define	XMEMPUT(src, size, p)	__xmemput__(src, size, (XVOIDPTR *) &(p))
#define	XMEMGET(dst, size, p)	__xmemget__(dst, size, (XVOIDPTR *) &(p))
#define	XNULL(p)		(!(p).xms.handle && !(p).mem.ptr)

/*
 *	Glue functions used by XMEMALLOC, XMEMFREE, XMEMPUT and XMEMGET
 */

extern int	__xmemalloc__(unsigned int size, XVOIDPTR * xptr);
extern int	__xmemfree__(XVOIDPTR * xptr);
extern int	__xmemput__(void * src, unsigned int size, XVOIDPTR * xptr);
extern int	__xmemget__(void * dst, unsigned int size, XVOIDPTR * xptr);

/*
 *	The union declared by XMEMPTR may be used to either declare
 *	a variable, or as part of a typedef, for example:
 *
 *	static XMEMPTR(char)	cptr;
 *
 *	declares a union called "cptr" which consists of an XMS pointer
 *	and a conventional memory pointer to char.  When used within a
 *	typedef, such as:
 *
 *	typedef XMEMPTR(int)	intptr;
 *
 *	the line containing the macro will expand as follows:
 *
 *	typedef union {
 *		_XMS_POINTER	xms;
 *		struct {
 *			_XMS_HANDLE	handle;
 *			int *		ptr;
 *		} mem;
 *	} intptr;
 *
 *	resulting in a declaration of a type "intptr" which can then be
 *	used with other variable declarations, for example:
 *
 *	static intptr	ip;
 *
 *	The "mem.handle" or "xms.handle" field can be used to determine
 *	whether XMS or conventional memory is being used, for example:
 *
 *	int	i, j;
 *
 *	XMEMALLOC(sizeof(int), ip);
 *	if (XNULL(ip))
 *		printf("XMEMALLOC failed!\n");
 *	else {
 *		i = 1;
 *		XMEMPUT(&i, sizeof(i), ip);
 *		XMEMGET(&jm sizoef(j), ip);
 *		printf("j = %d\n", j);
 *		XMEMFREE(ip);
 *	}
 */

#endif

/* 
 *	End of file: "$Id: xms.h,v 1.15 94/08/12 11:31:24 colin Exp $"
 */
