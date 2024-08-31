#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

int main(){
	int pipefds1[2], pipefds2[2], pipefds3[2];
	int returnstatus1, returnstatus2, returnstatus3;
	int pid;
	char msg[200];

	returnstatus1 = pipe(pipefds1);

	if(returnstatus1 == -1){
		printf("Unable to create pipe 1 \n");
		return 1;
	}

	returnstatus2 = pipe(pipefds2);

	if(returnstatus2 == -1){
		printf("Unable to create pipe 2 \n");
		return 1;
	}

	returnstatus3 = pipe(pipefds3);

	if(returnstatus3 == -1){
		printf("Unable to create pipe 3 \n");
		return 1;
	}

	pid = fork();

	if(pid){
		char word[200], reversed[200];
		bool palindromeCheck;

		close(pipefds1[0]);
		close(pipefds2[1]);
		close(pipefds3[1]);

		printf("Digite uma palavra: ");
		scanf("%s", word);

		write(pipefds1[1], word, strlen(word) + 1);
		close(pipefds1[1]);

		read(pipefds2[0], reversed, sizeof(reversed));
		close(pipefds2[0]);
		printf("%s\n", reversed);

		read(pipefds3[0], &palindromeCheck, sizeof(palindromeCheck));
		close(pipefds3[0]);

		if(palindromeCheck) printf("É palindromo!\n");
		else printf("Não é palíndromo!\n");

	}else if(pid == 0){
		close(pipefds1[1]);
		close(pipefds2[0]);
		close(pipefds3[0]);

		read(pipefds1[0], msg, sizeof(msg));
		close(pipefds1[0]);

		int len = strlen(msg);
		for(int i = 0; i < len/2; i++){
			char aux = msg[i];
			msg[i] = msg[len - 1 - i];
			msg[len - 1 - i] = aux;
		}
		msg[len] = '\0';

		write(pipefds2[1], msg, strlen(msg) + 1);
		close(pipefds2[1]);

		bool eh_palindromo = true;

		for(int i = 0; i < len/2; i++){
			if(msg[i] != msg[len-1-i]) eh_palindromo = false;
		}

		write(pipefds3[1], &eh_palindromo, sizeof(eh_palindromo));
		close(pipefds3[1]);
	}else{
		printf("Erro ao criar processo filho.\n");
		return 1;
	}

	return 0;
}
