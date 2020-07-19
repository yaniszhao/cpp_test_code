#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define KEYPATH "/root/chat"

#define NAMESIZE 16
#define MSGSIZE 128
#define LEN sizeof(int)+sizeof(int)+NAMESIZE+MSGSIZE

#define SERTYPE 1 
#define NORMAL 0
#define REGISTER 1
#define UNREGISTER -1

typedef struct node
{
    int id;
    struct node  *next;
}USR;

struct msgbuf
{
    long mtype;
    int subtype;
    int id;
    char nickname[NAMESIZE];
    char mtext[MSGSIZE];
}buf_in, buf_out;

int msgid;
USR *head;

USR * myregister(USR *head, int id)
{
   USR *p = (USR *)malloc(sizeof(USR));
   p->id = id;
   p->next = head->next;
   head->next = p;
   return head;
} 


int unregister(USR *head, int id)
{
   USR *p, *q;

   p = head;
   q = p->next;
 
   while( (q->id != id) && (q->next != NULL) )
   {
      p = q;
      q = q->next;
   }

   if ( q->id == id) 
   {
       p->next = q->next;
       free(q);
       return 1;
   }
  
   return 0;
}


int broadcast(int msgid, USR *head, int mode)
{
   USR *p;

   p = head->next;
   buf_out.subtype = mode;
   while( p != NULL)
   { 
      buf_out.mtype = p->id;
      if ( msgsnd(msgid, &buf_out, LEN, 0) != 0 )
      {
         perror("server send message error");
         exit(-1);
      }
      p = p->next;
   }
   
   return 1;
}

/*
void signal_handler()
{
   read(pfd[0], buf_out.mtext, 20); 
   strcpy(buf_out.nickname, "from server...");
   broadcast(msgid, head, NORMAL);
}  
*/

int main()
{
   int pid;
   key_t key;
   char command[20];

   //signal(SIGUSR1, signal_handler); 
   if ( (key=ftok(KEYPATH, 'c')) == -1 )
   {
      printf("faile to locate %s\n", KEYPATH);
      exit(-1);
   }

   msgid = msgget(key, IPC_CREAT|IPC_EXCL|0666);
   if ( msgid == -1 )
   {
      perror("msgget");
      exit(-1);
   }
/*
   if ( pipe(pfd) < 0 )
   {
      msgctl(msgid, IPC_RMID, NULL);
      perror("pipe...");
      exit(-1);
   }
*/   
   if ( (pid = fork()) == -1 )
   {
      msgctl(msgid, IPC_RMID, NULL);
      perror("fork");
      exit(-1);
   }

   if ( pid == 0 )
   {
      while ( 1 )
      {  
         printf("\n(server console):"); 
         if ( fgets(command, 20, stdin) != NULL )
         {
             if ( strncmp(command, "quit", 4) == 0 )
             {
                 printf("The server is down...\n");
                 kill(getppid(), SIGKILL);
                 sleep(1);
                 msgctl(msgid, IPC_RMID, NULL);
                 exit(0);
             }
             else
             {
                 buf_out.mtype = SERTYPE;
                 buf_out.subtype = NORMAL;
                 strcpy(buf_out.nickname, "from server...");
                 strcpy(buf_out.mtext, command);
                 if ( msgsnd(msgid, &buf_out, LEN, 0) < 0 )
                 {
                    perror("msgsnd");
                 }
                 //write(pfd[1], command, 10);
                 //kill(getppid(), SIGUSR1);
             }
         }
      }
   }
   else
   {
      head = (USR *)malloc(sizeof(USR));
      head->next = NULL;

      while(1)
      {
         if ( msgrcv(msgid, &buf_in, LEN, 1, 0) == -1 )
         {
            perror("msgrcv");
            exit(-1);
         }
         else
         {  
             buf_out = buf_in;
             switch(buf_in.subtype)
             {
                case UNREGISTER:
                         unregister(head, buf_in.id);
                         strcpy(buf_out.mtext, "has left...");
                         broadcast(msgid, head, UNREGISTER);                     
                         break;
                case  REGISTER:
                         head = myregister(head, buf_in.id);
                         strcpy(buf_out.mtext, "has joined in...");
                         broadcast(msgid, head, REGISTER); 
                         break;
                case  NORMAL: 
                         broadcast(msgid, head, NORMAL);
                         break;
             }
          }    // end of if
       }
   } 
}            
