#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#define N 64

int main(int argc, char *argv[])
{
	int ch, aflag = 0, lflag = 0, i;
	
	opterr = 0;
	if (argc == optind)
	{
		printf("no option\n");
		return 0;
	}

	while ((ch = getopt(argc, argv, "al")) != -1)
	{
		switch (ch)
		{
			case 'a':	aflag = 1;	break;
			case 'l':	lflag = 1;	break;
			default:
				printf("wrong option %c\n", optopt);
		}
	}

	printf("aflag = %d lflag = %d\n", aflag, lflag);
	printf("first para %s\n", argv[optind]);

	i = 0;
	while (i < argc)
		printf("%s ", argv[i++]);
	
	printf("\n");
	
	return 0;
}
