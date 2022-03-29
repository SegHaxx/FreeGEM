/* 	Test program - this is a single-precision floating point
 *	calculator. Type in expressions of the form A op B, where A and B
 *	are floating point numbers, and op is an operator. The usual
 *	arithmetic operators are recognized, plus = for comparision,
 *	s for sine, c for cosine and t for tan. With c, s and t only the
 *	first number need be given, e.g. 30 s will give the answer 0.50000.
 *
 *	To compile use PPD with "Use floating point library" enabled,
 *	or use PACC FCALC.C -LF
 */

#include	<ctype.h>
#include	<math.h>
#include	<stdlib.h>
#include	<stdio.h>
#include	<conio.h>

float	expr(void);

float	pi;

main()
{
	float	res;

	printf("\nPacific C Compiler calculator, enter a blank line to quit\n");
	pi = 4 * atan(1);
	for(;;) {
		res = expr();
		printf("Result = %f\n", res);
	}
}

float
expr()
{
	float	a, b;
	char *	cp;
	char	c;
	char	abuf[20];

	printf("FCALC> ");
	gets(abuf);
	if (!abuf[0])
		exit(0);
	cp = abuf;
	while(isspace(*cp))
		cp++;
	a = atof(cp);
	if(*cp == '-')
		cp++;
	while(isdigit(*cp) || *cp == 'e' || *cp == 'E' || *cp == '.')
		cp++;
	while(isspace(*cp))
		cp++;
	c = *cp;
	if(c == 0) {
		return a;
	}
	cp++;
	while(isspace(*cp))
		cp++;
	b = atof(cp);
	switch(c) {

	case 'c':
		return cos(a / 180.0 * pi);

	case 's':
		return sin(a / 180.0 * pi);

	case 't':
		return tan(a / 180.0 * pi);

	case '+':
		return a + b;

	case '-':
		return a - b;

	case '*':
		return a * b;

	case '/':
		return a / b;

	case '=':
		if(a < b)
			printf(" < ");
		if(a == b)
			printf(" == ");
		if(a > b)
			printf(" > ");
		if(a >= b)
			printf(" >= ");
		if(a <= b)
			printf(" <= ");
		putch('\n');
		return 0;

	default:
		return 0;
	}
}
