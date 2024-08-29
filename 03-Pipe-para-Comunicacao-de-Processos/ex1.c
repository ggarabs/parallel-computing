#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
	int pipefds1[2], pipefds2[2], pipefds3[2];
	int returnstatus1, returnstatus2, returnstatus3;
	int pid;
	char msg[5];

	returnstatus1 = pipe(pipefds1);
	returnstatus2 = pipe(pipefds2);
	returnstatus3 = pipe(pipefds3);

	pid = fork();

	char word[200], msg2[5], msg3[1];

	if(pid){
		scanf("%s", word);
		write(pipefds1[1], word, sizeof(word));
		read(pipefds2[0], msg, sizeof(msg2));
		printf("%s\n", msg);
		read(pipefds3[0], msg3, sizeof(msg3));

		if(msg3[0] == 'S'){
			printf("É palindromo!\n");
		}else{
			printf("Não é palíndromo!\n");
		}

	}else{
		read(pipefds1[0], msg, sizeof(msg));

		for(int i = 0; i < sizeof(msg)/2; i++){
			char aux = msg[i];
			msg[i] = msg[sizeof(msg) - 1 - i];
			msg[sizeof(msg) - 1 - i] = aux;
		}

		msg[sizeof(msg)] = '\0';

		write(pipefds2[1], msg, sizeof(msg));

		char eh_palindromo[1] = "S";

		for(int i = 0; i < sizeof(msg)/2; i++){
			if(msg[i] != msg[sizeof(msg)-1-i]) eh_palindromo[0] = 'N';
		}

		write(pipefds3[1], eh_palindromo, sizeof(eh_palindromo));
	}

	return 0;
}
