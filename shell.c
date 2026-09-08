#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
  while (1)
  {
    // array to store the input buffer 
	  char buffer[1024];

	  printf("$ ");

	  fgets(buffer, 1024, stdin);

    char *nl = strchr(buffer, '\n');

    if (nl) *nl = '\0';

    // splliting the command
    char *args[20];
    int cnt = 0;

    args[cnt] = strtok(buffer, " ");

    while (args[cnt] != NULL) 
    {
      args[++cnt] = strtok(NULL, " ");
    }

    if (strcmp(args[0], "exit") == 0) exit(0);

	  // running the command
    pid_t pid = fork();

    if (pid > 0)
    {
      // parent
      wait(NULL);
    }
    else
    {
      //child
      execvp(args[0], args);

      // exec error
      fprintf(stderr, "Could not exec %s\n", buffer);
    }
  }
}
