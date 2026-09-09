#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int handle_redirect(char *args[]);

int main(int argc, char *argv[])
{
  // 1 - interactive
  int mode = 1;
  if (argc > 1)
  {
    // argc[0] - name of script
    if(freopen(argv[1], "r", stdin) == NULL)
    {
      fprintf(stderr, "Can't read from script file %s\n",argv[1]);
      fprintf(stderr, "Existing..\n");
      exit(1);
    }
    mode = 0;
  }
  while (1)
  {
    // array to store the input buffer 
	  char buffer[1024];

	  if(mode) printf("$ ");

	  if(fgets(buffer, 1024, stdin) == NULL) break;

    char *nl = strchr(buffer, '\n');

    if (nl) *nl = '\0';

    // Remove comments
    char *hash = strchr(buffer, '#');

    if (hash) *hash = '\0';

    // splliting the command
    char *args[20];
    int cnt = 0;

    args[cnt] = strtok(buffer, " ");

    while (args[cnt] != NULL) 
    {
      args[++cnt] = strtok(NULL, " ");
    }

    if (args[0] == NULL) continue;

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
      if(handle_redirect(args) == -1)
      {
        fprintf(stderr, "could not redirect\n");
        exit(1);
      }
      //child
      execvp(args[0], args);

      // exec error
      fprintf(stderr, "Could not exec %s\n", buffer);
    }
  }
}

// handle redirect stdout
// 1 for success
// 0 for no redirect
// -1 for error
int handle_redirect(char *args[])
{
  for (int i = 0; args[i] != NULL; i++)
  {
    if (strcmp(args[i], ">") == 0)
    {
      // args[i + 1] is the file to write to
      if(freopen(args[i + 1], "w", stdout) == NULL) return -1;

      if (stdout == NULL) return -1;

      args[i] = NULL;

      return 1;
    }
  }
  return 0;
}
