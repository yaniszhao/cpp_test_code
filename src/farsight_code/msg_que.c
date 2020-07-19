#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE	512

struct message {
	long msg_type;
	char masg_text[BUFFER_SIZE];
};

int main(void) 
{
	int qid;
	key_t key;
	struct message msg;
	
	if (-1 == (key = ftok(".", 'a'))) {
		perror("ftok");
		exit(1);
	}
	
	if (-1 == msgget(key, IPC_CREAT|0666)) {
		perror("msgget");
		exit(1);		
	}
	
	printf("Open queue %d\n", qid);
	
	while (1) {
		printf("Enter some message to the queue:");
		if (NULL == fgets(msg.msg_text, BUFFER_SIZE, stdin)) {
			puts("no message");
			exit(1);
		}
		
		msg.msg_type = getpid();
		
		if (0 > msgsnd(qid, &msg, strlen(msg.msg_text))) {
			perror("message posted");
			exit(1);
		}
		
		if (0 == strncmp(msg.msg_text, "quit", 4)) {
			break;
		}			
	}
	
	exit(0);
}

/* ================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE	512

struct message {
	long msg_type;
	char masg_text[BUFFER_SIZE];
};

int main(void)
{
		int qid;
		key_t key;
		struct message msg;
		
		if (-1 == (key = ftok(".", 'a'))) {
			perror("ftok");
			exit(1);
		}
	
		if (-1 == (qid = msgget(key, IPC_CREAT|0666))) {
			perror("msgget");
			exit(1);
		}
		
		printf("Open queue %d\n", qid);
		
		do {
			memset(msg.msg_text, 0, BUFFER_SIZE);
			
			if (0 > msgrcv(qid, (void *)&msg, BUFFER_SIZE, 0, 0)) {
				perror("msgrcv");
				exit(1);
			}
			
			printf("The message from process %d : %s",
						msg.msg_type, msg.msg_text);
		} while (strncmp(msg,msg_text, "quit", 4));
		
		if (0 > msgctl(qid, IPC_RMID, NULL)) {
			perror("msgctl");
			exit(1);
		}
		
		exit(0);
}
