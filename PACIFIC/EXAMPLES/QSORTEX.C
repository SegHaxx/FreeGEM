#include	<stdio.h>
#include	<stdlib.h>
#include	<time.h>

/*
 *	Pacific C QSORT Example.
 *
 *	This program demonstrates the use of the qsort() function
 *	to sort an array.  It also includes an example of random
 *	number generation using the rand() function.  Note the use
 *	of the system time (obtained using time()) to seed the
 *	random number generator via the srand() function.
 */

#define	ARRAY_SIZE	64

int	array[ARRAY_SIZE];

/*
 *	void	make_array(void)
 *
 *	Generate ARRAY_SIZE random elements of "array[]" using the
 *	rand() function.  Note use of time() and srand() to seed
 *	the random number generator.
 */

static void
make_array(void)
{
	int	index;
	time_t	seed;

	time(&seed);	/* use system time as random seed */
	srand(seed);
	for (index = 0; index != ARRAY_SIZE; index++)
		array[index] = rand();
}

/*
 *	void	print_array(void)
 *
 *	Print "array[]" in rows of eight columns.
 */

static void
print_array(void)
{
	int	index, col;

	col = 0;
	for (index = 0; index != ARRAY_SIZE; index++) {
		printf("%8d", array[index]);
		if (++col == 8) {
			putchar('\n');
			col = 0;
		}
	}
	if (col != 0)
		putchar('\n');
}

/*
 *	int	compare_ints(const void * i1, const void * i2)
 *
 *	"Compare function" used by qsort() to compare array elements.
 *	qsort() calls this function with the "const void *" arguments
 *	set to point to a pair of array elements.  To dereference the
 *	"const void *" arguments, a cast to the appropriate type (int *)
 *	must be performed.
 *
 *	compare_ints returns the following values:
 *
 *	0			If *i1 == *i2
 *	negative integer	If *i1 < *i2
 *	positive integer	If *i1 > *i2
 */

static int
compare_ints(const void * i1, const void * i2)
{
	return (*(int *) i1) - (*(int *) i2);
}

/*
 *	main program.
 */

main()
{
	printf("Pacific C QSORT example\n");
	printf("Generating randomised array\n");
	make_array();
	printf("Array before QSORT:\n");
	print_array();
	printf("Sorting ...\n");
	qsort(array, ARRAY_SIZE, sizeof(array[0]), compare_ints);
	printf("Array after QSORT:\n");
	print_array();
}
