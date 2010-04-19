// This is simple set of coding rules for our project.
// Not compulsory, but desirable.

//**********************************************************************
// how you can write:
if (something)
{
	dosomething();
}
// how you can't write:
if(something) // we need a space before an open bracket!
{
	dosomething();
}

// you can write like this too:
if (something) {
	dosomething();
}

// the same with 'while', 'for', 'do', 'switch'
//**********************************************************************

//**********************************************************************
int foo(double a, double *b)
{
	int x = 0;
	/* empty line! */
	*b = a;
	x = 2 + 2 - (7 - 22 * 4 % 78);
	/* empty line! */
	return x;
}

void bar(void) {
}
//**********************************************************************
