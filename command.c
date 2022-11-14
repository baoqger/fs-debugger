#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "command.h"

COMMAND commands[] = {
    {"open", com_open, "Open file system"},
    {"superblock", com_superblock, "Display the content of superblock."},
    {"inode", com_inode, "Dump the inode inforamtion."},
    {"block", com_block, "Dump the data block."},
    {"help", com_help, "Display the help of fsdb."},
    {"?", com_help, "Synonym for 'help'."},
    {"quit", com_quit, "Quit using fsdb."},
    {(char*)NULL, (rl_icpfunc_t *)NULL, (char *)NULL}
};

/* Execute a command line */
int execute_line(char *line) {
    register int i; 
    COMMAND *command;
    char *word; 
    
    i = 0;
    // strip the beginning white space
    while(line[i] && isspace(line[i]))  
        i++;
    word = line + i; // point to the first character of command word 

    // move the pointer until the first space or the end('\0') of the string
    while (line[i] && !isspace(line[i]))
        i++;
    
    // if the pointer reaches whitespace instead of the end
    // add the terminated char manually 
    if (line[i]) {
        line[i++] = '\0';
    }

    command = find_command(word);

    if (!command) {
        fprintf(stderr, "%s: No such command for Fileman.\n", word);
        return -1;
    }

    // Get the argument to the command if any. Support only one argument.
    while(isspace(line[i])) 
        i++;

    word = line + i; // now word points to the first character of argument

    // call the function
    return command->func(word);
}

// Look up NAME as the name of a command, and return a pointer to that command.
// Return a NULL pointer if NAME 
COMMAND* find_command(char *name) {
    register int i;

    for (i = 0; commands[i].name; i++) {
        if(strcmp(name, commands[i].name) == 0)
            return &commands[i];
    }
    
    return NULL; 
}

// Strip whitespace from the start and end of STRING. Return a pointer to STRING
char* stripwhite(char *string) {
    register char *s, *t;
    
    // strip the whitespace at the beginning
    for (s = string; isspace(*s); s++)
        ;

    // judge whether s already points to the terminated character '\0'
    if (*s == 0) {
        return s;
    }

    t = s + strlen(s) - 1; // t points to the last character in the string
    // strip the whitespace at the end
    while(t > s && isspace(*t))
        t--;
    *(++t) = '\0'; // same as *(++t) = 0

    return s;
}

/*
 * ***************** Interface to Readline Completion *************************
 * */

/*
 * Tell the GNU Readline library how to complete. We want to try to complete
 * on command names if this is the first word in the line, or on filenames if not
 * */
void initialize_readline() {
   // Allow conditional parsing of the /.inputrc file./
   rl_readline_name = "FileMan";

   // Tell the completeer that we want a crack first
   rl_attempted_completion_function = fsdb_completion;
}

/*
 * rl_attempted_completion_function: A pointer to an alternative function to create matches.
 * The function is called with text, start, end. 
 * start and end are indices in rl_line_buffer defining the boundaries of text, which is a character string. 
 * If this function exists and return NULL, or if this variable is set to NULL, then rl_complete() will call the
 * value of rl_complete_entry_function to generate matches. otherwise the array of strings returned will be used
 * */

char** fsdb_completion(const char *text, int start, int end) {
    char **matches = NULL;
    /*
     * If this word is at the start of the line, then it is a command to complete.
     * otherwise it is the name of a file in the current directory
     * */
    if (start == 0) {
        matches = rl_completion_matches(text, command_generator);
    }
    return matches;
}

/*
 * Generator function for command completion. STATE lets us know whether
 * to start from scratch; without any state (i.e. STATE == 0), then we start at 
 * the top of the list
 * */

char* command_generator(const char *text, int state) {
    static int list_index, len;
    char *name;

    /*
     * If this is new word to complete, initialize now.
     * This includes saving the length of TEXT for efficiency, and initializing 
     * the index variable to 0
     * */

    if (!state) {
        list_index = 0;
        len = strlen(text);
    }

    // Return rthe next name which partially matches from the command list
    while((name = commands[list_index].name) != NULL) {
        list_index++;
        if (strncmp(name, text, len) == 0) {
            return strdup(name);
        }
    }

    // If no names matched, then return NULL
    return NULL;
}

/*****************************fsdb command*****************************/

// print out help for ARG, or for all of the commands if ARG is not present
int com_help (char *arg) {
    register int i;
    int printed = 0;
    for (i = 0; commands[i].name; i++) {
        if(!*arg || strcmp(arg, commands[i].name) == 0) {
            printf("%s\t\t%s.\n", commands[i].name, commands[i].doc);
            printed++;
        } 
    }
    if (!printed) {
        printf("No commands match '%s'. Possibilities are: \n", arg);
        for (i = 0; commands[i].name; i++) {
            // print in six columns
            if (printed == 6) {
                printed = 0;
                printf("\n");
            }
            printf("%s\t", commands[i].name);
            printed++;
        }
        if (printed)
            printf("\n");
    }
    return 0;
}

// The user wishes to quit using this program. Just set DONE non-zero
int com_quit(char* arg) {
    done = 1;
    return 0;
}

int com_open(char* arg) {
    return 0;
}

int com_superblock(char* arg) {
    return 0;
}

int com_inode(char* arg) {
    return 0;
}

int com_block(char* arg) {
    return 0;
}