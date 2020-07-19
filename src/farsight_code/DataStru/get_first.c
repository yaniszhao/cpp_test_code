#include <stdio.h>
//得到第一个只出现一次的小写字母
char get_first(const char *str)
{
	int num[26] = {0};
	const char *p = str;

	while (*p) {
		num[*p-'a']++;
		p++;
	}

	while (*str) {
		if(1 == num[*str-'a'])
			return *str;
		str++;
	}
	
	return 'X';
}

int main()
{
	char *p = "xaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaasdfasdfasdfasdtgdfh";

	char c = get_first(p);

	printf("%c\n", c);
}
