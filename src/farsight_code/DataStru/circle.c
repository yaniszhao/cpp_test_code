#include <stdio.h>

#define PEOPLE_NUM 13 
#define makesure()\
do{\
	while (peo[n-1] == -1)\
	{\
		++n;\
		if (n > PEOPLE_NUM)\
			n=1;\
	}\
}while(0);
#define next()\
do{\
	++n;\
	if (n > PEOPLE_NUM)\
		n=1;\
}while(0);
int main()
{
        int peo[PEOPLE_NUM];
        int i;
        int num = PEOPLE_NUM;
        int n = 1;
        for (i = 0; i < PEOPLE_NUM; ++i)
                peo[i] = i+1;

        while (num > 1)
		{
			makesure();
			next();
			makesure();
			next();
			makesure();

			peo[n-1] = -1;
			--num;
			next();
			printf("n = %d\n", n);
			printf("num = %d\n", num);
			for (i = 0; i < PEOPLE_NUM; ++i)
                printf("%d,",peo[i]);
			printf("\n");

        }

        makesure();
        printf("%d\n", n);


}

