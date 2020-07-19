#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <time.h>

#define KEYPATH "/root/chat"

#define NAMESIZE 16
#define MSGSIZE 128
#define LEN sizeof(int)+sizeof(int)+NAMESIZE+MSGSIZE

#define SERTYPE 1

// subtype
#define NORMAL 0
#define REGISTER 1
#define UNREGISTER -1


struct msgbuf
{
    long mtype;
    int subtype;
    int id;
    char nickname[NAMESIZE];
    char mtext[MSGSIZE];
}buf_in, buf_out;



int main(int argc, char **argv)
{
   int msgid,pid;
   key_t key;

   if ( argc != 2 )
   {
      fprintf(stdout, "Usage: %s <nickname> \n", argv[0]);
      exit(1);
   }

   if ( (key = ftok(KEYPATH, 'c')) == -1 )
   {
      printf("faile to locate %s\n", KEYPATH);
      exit(-1);
   }

   msgid = msgget(key, 0666);
   if ( msgid == -1 )
   {
      printf("Be sure server is on...\n");
      exit(-1);
   }

// register to the server
   buf_out.mtype = SERTYPE;
   buf_out.subtype = REGISTER;
   buf_out.id = getpid();
   strcpy(buf_out.nickname, argv[1]);
   memset(buf_out.mtext, 0, MSGSIZE);

   if ( msgsnd(msgid, &buf_out, LEN, 0) == -1 )
   { 
      perror("Failed to register");
      exit(-1);
   }

   if ( (pid = fork()) == -1 )
   {
      perror("fork:");
      exit(-1);
   }

   if ( pid > 0 )   // parent process
   {
      while(1)
      {
         printf("\nPlease input message:"); 
         fgets(buf_out.mtext, MSGSIZE, stdin);
         if ( strncmp(buf_out.mtext,"quit",4) == 0 )
         {
            kill(pid, SIGQUIT);
            buf_out.subtype = UNREGISTER;
            if ( msgsnd(msgid, &buf_out, LEN, 0) == -1 )
            {   
               perror("can't send message...");
            }  
            exit(0);
         }
         buf_out.subtype = NORMAL;
         if ( msgsnd(msgid, &buf_out, LEN, 0) == -1 )
         {
            perror("can't send message..."); 
         } 
      }
   }
   else                  // child process
   {
      while ( 1 )
      {
         if ( msgrcv(msgid, &buf_in, LEN, getppid(), 0) == -1 )
         {
            printf("\ncan't receive message, maybe server is down. exit...\n");
            kill(getppid(), SIGINT);
            sleep(1);
            exit(-1);
         } 
         if ( buf_in.subtype == NORMAL )
         { 
             printf("\n[%s] %s\n", buf_in.nickname, buf_in.mtext);
         }
         else
         {
             printf("\n[%s %s]\n", buf_in.nickname, buf_in.mtext); 
         }      
      }
   }
}            
