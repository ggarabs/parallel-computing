#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(void) {
   pid_t childpid;

   childpid = fork();
   if (childpid == -1) {
      perror(Failed to fork);
      return 1;
   }

   if (childpid == 0)      /* child code */
      printf(I am child %ldn, (long)getpid());
   else                    /* parent code */
      printf(I am parent %ldn, (long)getpid());
   return 0;
}
