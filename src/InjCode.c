#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define bool int
#define true 1
#define false 0
#define USAGE_WORDS "Usage: InjCode add/del file\n"
#define BAD_FILENAME_WORDS "error:file name is bad\n" 
#define SUFFIX ".bak.zy"
#define BUFFER_LEN 1024
#define MAX_FILE_LEN (BUFFER_LEN - strlen(SUFFIX) - 1)
#define MAGIC htonl(0x11223344)

int ReadMagic(int fd, void *buf, int len)
{
	if (read(fd, buf, len) != len)
	{
		printf("error:read buf\n");
		return -1;
	}
	return 0;
}

int main(int argc, char *argv[])
{

	if (argc != 3)
	{
		printf(USAGE_WORDS);
		exit(0);
	}

	bool AddFlag = false;
	char *opt = argv[1];
	char *fileName =  argv[2];
	int fd = 0xFFFFFFFF;
	int magVal = 0;
	if (!strcmp(opt, "add"))
	{
		AddFlag = true;
		printf("chosen add\n");
	} 
	else if (!strcmp(opt, "del"))
	{
		AddFlag = false;
		printf("chosen del\n");
	}
	else
	{
		printf(USAGE_WORDS);
		exit(0);
	}

	if (access(fileName, 0))
	{
		printf(BAD_FILENAME_WORDS);
		exit(0);
	}

	if ((fd = open(fileName, O_RDWR, 0)) == -1)
	{
		printf("error:open file fail\n");
		exit(0);
	}

	ReadMagic(fd, (void*)&magVal, sizeof(magVal));
	close(fd);
	printf("magVal=0x%08x\n", ntohl(magVal));

	if (AddFlag)
	{
		char newFileName[BUFFER_LEN];
		char buf[BUFFER_LEN];
		int magic = MAGIC;
		int len = ((strlen(fileName) <= MAX_FILE_LEN) ? strlen(fileName) : MAX_FILE_LEN);
		int fdSrc = 0xFFFFFFFF;
		int fdDst = 0xFFFFFFFF;
		int ret = 0;

		if (magVal == MAGIC)
		{
			printf("error:magic exist\n");
			exit(0);
		}

		memcpy(newFileName, fileName, len);
		strcat(newFileName, SUFFIX);
		if (creat(newFileName, S_IRUSR|S_IWUSR|S_IXUSR) == -1)
		{
			printf("error:create file fail\n");
			exit(0);
		}
		printf("create file ok\n");

		if ((fdSrc = open(fileName, O_RDONLY, 0)) == -1)
		{
			printf("error:open fdSrc fail\n");
			exit(0);
		}
		
		if ((fdDst = open(newFileName, O_WRONLY, 0)) == -1)
		{
			printf("error:open fdDst fail\n");
			exit(0);
		}

		if (write(fdDst, &magic, sizeof(magic)) != sizeof(magic))
		{
			printf("error:write magic fail\n");
			exit(0);
		}

		while ((ret = read(fdSrc, buf, BUFFER_LEN)))
		{
			if (ret < 0)
			{
				printf("error:read fdSrc fail\n");
				exit(0);
			}
			else if (ret == 0)
			{
				//EOF
				break;
			}
			else
			{
				if (write(fdDst, &buf, ret) != ret)
				{
					printf("error:write magic fail\n");
					exit(0);
				}
			}
		}

		printf("write ok\n");
		close(fdSrc);
		close(fdDst);

		if (rename(newFileName, fileName) == -1)
		{
			printf("error:rename fail\n");
			exit(0);
		}
		printf("rename ok\n");
	}
	else
	{
		char newFileName[BUFFER_LEN];
		char buf[BUFFER_LEN];
		int len = ((strlen(fileName) <= MAX_FILE_LEN) ? strlen(fileName) : MAX_FILE_LEN);
		int fdSrc = 0xFFFFFFFF;
		int fdDst = 0xFFFFFFFF;
		int ret = 0;
		if (magVal != MAGIC)
		{
			printf("error:magic not exist\n");
			exit(0);
		}

		memcpy(newFileName, fileName, len);
		strcat(newFileName, SUFFIX);
		if (creat(newFileName, S_IRUSR|S_IWUSR|S_IXUSR) == -1)
		{
			printf("error:create file fail\n");
			exit(0);
		}
		printf("create file ok\n");

		if ((fdSrc = open(fileName, O_RDONLY, 0)) == -1)
		{
			printf("error:open fdSrc fail\n");
			exit(0);
		}
		
		if ((fdDst = open(newFileName, O_WRONLY, 0)) == -1)
		{
			printf("error:open fdDst fail\n");
			exit(0);
		}

		if (read(fdSrc, buf, sizeof(magVal)) != sizeof(magVal))
		{
			printf("error:read magic fail\n");
			exit(0);
		}

		while ((ret = read(fdSrc, buf, BUFFER_LEN)))
		{
			if (ret < 0)
			{
				printf("error:read fdSrc fail\n");
				exit(0);
			}
			else if (ret == 0)
			{
				//EOF
				break;
			}
			else
			{
				if (write(fdDst, &buf, ret) != ret)
				{
					printf("error:write magic fail\n");
					exit(0);
				}
			}
		}

		printf("write ok\n");
		close(fdSrc);
		close(fdDst);

		if (rename(newFileName, fileName) == -1)
		{
			printf("error:rename fail\n");
			exit(0);
		}
		printf("rename ok\n");
	}

	return 0;
}