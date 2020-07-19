#include <stdio.h>
#include <string.h>
#include <errno.h>
#define N 10
int main(int argc,char *argv[])
{
	FILE *fp1,*fp2;
	int line=0;
	char buf[N];
	if(argc <3)
	{
		fprintf(stdout,"Using %s srcfilename  desfilename\n",argv[0]);
		return 0;
	}
	
	if ((fp1=fopen(argv[1],"r"))==NULL)
	{
		fprintf(stderr,"open %s failed:%s\n",argv[1],strerror(errno));
		return -1;
	}
	
	if((fp2 = fopen(argv[2],"w"))==NULL)
	{
		fprintf(stderr,"open %s failed:%s\n",argv[2],strerror(errno));
		fclose(fp1);
		return -1;
	}

	while(fgets(buf,N,fp1)!=NULL)
	{
		fputs(buf,fp2);
		//if((buf[N-2]=='\n') || (buf[N-2]=='\0'))
		if((strlen(buf)<(N-1))||(buf[N-2]=='\n'))
				line++;
	}
	/*	while((c= fgetc(fp1))!=EOF)
		{
			fputc(c,fp2);
		}	
	 */
	 /*	while((t=fread(buf,1,10,fp1))==10)
		{
			fwrite(buf,1,t,fp2);
		}
		fwrite(buf,1,t,fp2);	 
	  */
	printf("Line:%d\n",line);
	fclose(fp1);
	fclose(fp2);
	return 0;

}
