// filename: ex1_childprocess.c
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#define BUF_SIZE 1023
#define SHM_KEY 0x1234

struct shmseg {
   int cnt;
   int complete;
   char buf[BUF_SIZE];
   char palindrome;
};

int main(int argc, char *argv[]) {
   int shmid;
   struct shmseg *shmp;
   shmid = shmget(SHM_KEY, sizeof(struct shmseg), 0644|IPC_CREAT);
   if (shmid == -1) {
      perror("Shared memory");
      return 1;
   }

   shmp = shmat(shmid, NULL, 0);
   if (shmp == (void *) -1) {
      perror("Shared memory attach");
      return 1;
   }

   while (shmp->complete != 1) {
	   printf("Aguardando escrita...\n");
	   sleep(2);
   }

   printf("Leitura completa! Palavra: %s\n\n", shmp->buf);

   printf("Invertendo palavra...\n");

   for(int i = 0; i < strlen(shmp->buf)/2; i++){
	   char aux = shmp->buf[i];
	   shmp->buf[i] = shmp->buf[strlen(shmp->buf)-1-i];
	   shmp->buf[strlen(shmp->buf)-1-i] = aux;
   }

   printf("Inversão finalizada! Palavra: %s\n\n", shmp->buf);

   shmp->complete = 0;

   while(shmp->complete != 1){
	printf("Aguardando verificação de palindromo...\n");
	sleep(2);
   }

   if(shmp->palindrome == 'S') printf("A palavra digitada é palindromo!\n");
   else printf("A palavra digitada não é palindromo!\n");

   shmdt(shmp);

   return 0;
}
