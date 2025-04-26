#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <fcntl.h>

bool in_background = false;
pid_t executing_program = 0;

char *read_line(void)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread; //length of line

    if ((nread = getline(&line, &len, stdin)) == -1){
        if (feof(stdin)) { // checks if ctrl+d was pressed (EOF) and terminates
        printf("\n");
        exit(EXIT_SUCCESS);
        } else  {
        perror("readline");
        exit(EXIT_FAILURE);
        }
    }

    if(nread > 1 && line[nread-2] == '&') //last character before EOF
    {
        line[nread-2] = '\0';
        in_background = true;
    }

    return line;
}

#define TOK_BUFSIZE 64
#define TOK_DELIM " \t\n"
char **split_line(char *line, int *position, char **delim)
{
  int bufsize = TOK_BUFSIZE, token_position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;

  if (!tokens) {
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  if(*position == 0) token = strtok(line, TOK_DELIM); //strtok extracts tokens from strings
  else token = strtok(NULL, TOK_DELIM);

  while (token != NULL && strcmp(token, "|") != 0 && strcmp(token, "<") != 0 && strcmp(token, ">") != 0 && strcmp(token, "2>") != 0) {
    tokens[token_position] = token;
    (*position)++;
    token_position++;

    if (token_position >= bufsize) {
      bufsize += TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, TOK_DELIM);
  }
  *delim = token; //assign to delim | < > 2> or NULL
  if(token == NULL){
    *position = -1;
  }
  tokens[token_position] = NULL;
  return tokens;
}

void *get_file(char **infile, char **outfile, char **errfile, char *line, int *position, char **delim)
{
  char **args;
  *infile = NULL;
  *outfile = NULL;
  *errfile = NULL;

  while(*delim != NULL && (strcmp(*delim, "<") == 0 || strcmp(*delim, ">") == 0 || strcmp(*delim, "2>") == 0)){
    if(strcmp(*delim, "<") == 0)
    {
      args = split_line(line, position, delim);
      *infile = args[0];
    }
    else if(strcmp(*delim, ">") == 0)
    {
      args = split_line(line, position, delim);
      *outfile = args[0];
    }
    else if(strcmp(*delim, "2>") == 0)
    {
      args = split_line(line, position, delim);
      *errfile = args[0];
    }
    if(*delim != NULL && (strcmp(*delim, "<") == 0 || strcmp(*delim, ">") == 0 || strcmp(*delim, "2>") == 0))
    {
      free(args);
    }
  }
}

/*
  Function Declarations for builtin shell commands:
 */
int cd(char **args);
int exit_sh(char **args);

/*
  List of builtin commands, followed by their corresponding functions.
 */
char *builtin_str[] = {
  "cd",
  "exit"
};

int (*builtin_func[]) (char **) = {
  &cd,
  &exit_sh
};

int num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

/*
  Builtin function implementations.
*/
int cd(char **args)
{
  if (args[1] == NULL) {
    fprintf(stderr, "lsh: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("lsh");
    }
  }
  return 1;
}

int exit_sh(char **args)
{
  return 0;
}

void assign_redirections(char **infile, char **outfile, char **errfile)
{
    // Handle input redirection
    if (*infile) {
      // man 2 open
      int fd = open(*infile, O_RDONLY); //O_RDONLY - read only
      if (fd < 0) {
          perror("Failed to open input file");
          exit(1);
      }
      dup2(fd, STDIN_FILENO); //plug file's fd to stdin of child
      close(fd);
    }

    // Handle output redirection
    if (*outfile) {
      // write-only, create file if not exists, if file exists, truncate to len 0
      // 0644 are file permissions. Here it's Owner has read/write perm, group and others have read-only perm
      int fd = open(*outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
      if (fd < 0) {
          perror("Failed to open output file");
          exit(1);
      }
      dup2(fd, STDOUT_FILENO);
      close(fd);
    }

    // Handle error redirection
    if (*errfile) {
      int fd = open(*errfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
      if (fd < 0) {
          perror("Failed to open error file");
          exit(1);
      }
      dup2(fd, STDERR_FILENO);
      close(fd);
    }
}

int pipefd[2];
int last_pipefd[2] = {-1, -1};
int execute_commands(char **args, char **delim, char **last_delim, char **infile, char **outfile, char **errfile)
{
  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }

  for (int i = 0; i < num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

  int j;
  pid_t pid, wpid;
  int status;

  if(*delim)
  {
    j = pipe(pipefd);
    if(j < 0){
      perror("lss: pipe");
      exit(1);
    }
  }

  pid = fork();
  if (pid == 0) 
  {
    assign_redirections(infile, outfile, errfile);

    // Child process
    if(*last_delim){
      if(dup2(last_pipefd[0], 0) == -1){
        perror("lss: dup2(last_pipefd[0])");
        exit(1);
      }
      close(last_pipefd[1]);
      close(last_pipefd[0]);
    }

    if(*delim){
      if(dup2(pipefd[1], 1) == -1){ //write end of pipe to stdout
        perror("lss: dup2(pipefd[1])");
        exit(1);
      }
      close(pipefd[1]);
      close(pipefd[0]);
    }

    if (execvp(args[0], args) == -1) {
      perror("lsh");
      exit(1);
    }
    exit(EXIT_FAILURE);
  } 
  else if (pid < 0) 
  {
    // Error forking
    perror("lsh");
  } else 
  {
    if(*delim){
      if (last_pipefd[0] != -1) close(last_pipefd[0]);
      if (last_pipefd[1] != -1) close(last_pipefd[1]);
      last_pipefd[0] = dup(pipefd[0]);  // Duplicate write end
      last_pipefd[1] = dup(pipefd[1]);  // Duplicate read end
      close(pipefd[1]);
      close(pipefd[0]);
    }
    else {
    if (last_pipefd[0] != -1) close(last_pipefd[0]);
    if (last_pipefd[1] != -1) close(last_pipefd[1]);
    last_pipefd[0] = -1;
    last_pipefd[1] = -1;
    }
    // Parent process
    if(in_background){
        return 1;
    }
    executing_program = pid;
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    executing_program = 0;
  }

  return 1;
}

/*
SIGCHLD is sent to a parent process whenever one of its child processes changes state. 
*/
void handle_sigchld(int sig) {
  (void)sig; // Suppress 'unused variable warning' in -Wall

  pid_t pid;
  int status;

  // Reap all terminated child processes
  while ((pid = waitpid(-1, &status, WNOHANG)) > 0);
}

void handle_sigint(int sig){ 
  (void)sig;

  if(executing_program != 0)
  {
    if (kill(executing_program, SIGKILL) == -1) {
        fprintf(stderr, "Could not terminate program of PID: %d\n", executing_program);
    }
  }
}

int main()
{
    signal(SIGCHLD, handle_sigchld); //handle killing zombie child processes
    signal(SIGINT, handle_sigint); //handle ctrl+c

    char *line;
    char **args;
    char *delim;
    char *last_delim = NULL;
    char *infile;
    char *outfile;
    char *errfile;
    int status = 1;
    int position;

    do {
      printf("$ ");

      in_background = false;
      position = 0;

      line = read_line();

      while(status && position != -1)
      {
        args = split_line(line, &position, &delim);
        get_file(&infile, &outfile, &errfile, line, &position, &delim);
        status = execute_commands(args, &delim, &last_delim, &infile, &outfile, &errfile);
        last_delim = delim;
        free(args);
      }

      free(line);
    } while (status);

    free(delim);
    free(last_delim);

    return EXIT_SUCCESS;
}
