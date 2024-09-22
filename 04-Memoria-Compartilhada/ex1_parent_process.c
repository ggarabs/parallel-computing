// filename = ex1_parentprocess.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
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

int main() {
    int shmid = shmget(SHM_KEY, sizeof(struct shmseg), 0644|IPC_CREAT);
    struct shmseg *shmp;

    if (shmid == -1) {
        perror("Erro ao criar segmento de memória compartilhada");
        return 1;
    }

    shmp = shmat(shmid, NULL, 0);

    if (shmp == (void *) -1) {
        perror("Erro ao anexar memória compartilhada");
        return 1;
    }

    shmp->complete = 0;
    printf("Digite uma string: ");
    fgets(shmp->buf, BUF_SIZE, stdin);
    shmp->buf[strlen(shmp->buf)-1] = '\0';
    shmp->complete = 1;

    char word[BUF_SIZE];
    strcpy(word, shmp->buf);

    printf("String escrita na memória compartilhada: %s\n", shmp->buf);

    while(shmp->complete != 0){
	    printf("Aguardando inversão...\n");
	    sleep(2);
    }

    printf("Leitura completa! Palavra invertida: %s\n", shmp->buf);

    printf("Verificando se a palavra é palindromo...\n");

    shmp->palindrome = 'S';
    for(int i = 0; i < strlen(shmp->buf); i++){
	    if(shmp->buf[i] != shmp->buf[strlen(shmp->buf)-1-i]) shmp->palindrome = 'N';
    }
    shmp->complete = 1;

    printf("Verificação concluída!\n");

    shmdt(shmp);

    return 0;
}

