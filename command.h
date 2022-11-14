#ifndef COMMAND_H
#define COMMAND_H 

#include <readline/readline.h>
#include <readline/history.h>

extern int done;

typedef struct {
    char *name;         // User printable name of the function
    rl_icpfunc_t *func; // Function to call to do the job
    char *doc;
} COMMAND;

/* The names of functions that actually do the manipulation. */
int com_open(), com_superblock(), com_inode(), com_block(), com_help(), com_quit();

char* stripwhite(char*);
COMMAND* find_command(char*);
void initialize_readline();
int execute_line(char*);
int valid_argument(char*, char*);

char* command_generator(const char*, int);
char** fsdb_completion(const char*, int, int);

#endif 
