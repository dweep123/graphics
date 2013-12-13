#include <stdio.h>

int main(void)
{
	int num = 123;

	char buf[12];
	sprintf(buf, "%d", num);

	printf("%s\n", buf);

	return 0;
}
