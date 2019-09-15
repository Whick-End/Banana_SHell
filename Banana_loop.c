#include "Banana_shell.h"


// Extern variable, to know if the shell continue
char shell_continue;

int banana_loop(void) {

    int i;
    long int pipe_command_size = BUF_SIZE;
    char **check_env = NULL, **line_clean = NULL, **line_2_commands = NULL;
    char *line = NULL, *prompt = NULL;

    // Catch Control-C
    (void)signal(SIGINT, handler);

    if (errno)
        return EOF;

    if (setenv("SHELL", "Banana SHEll", 1) == EOF || errno)
        return EOF;

    // By default
    shell_continue = TRUE;

    // Loop, while user do not want exit the shell (by exit command or Ctrl-D)
    while (shell_continue) {
        
        // Print the shell interface
        if (!(prompt = get_banana_shell()) || errno)
            return EOF;

        // Get user input
        line = get_input(prompt);
        
        // If get_input failed
        if (errno)
            return EOF;

        // If Control-D is pressed
        if (!line) {

            (void)putc('\n', stdout);
            shell_continue = FALSE;

        }

        // If Control-D ISN'T pressed, exit
        else {

            // If the input is "enter" continue without starting processes commands, ...
            if (*line == '\n')
                return 1;

                // Execute command(s)

                // If any pipe(s)
            if (strchr(line, '|')) {
                
                /***
                * 
                * If User enter pipe,
                * pipe_command is egal to all the commands without pipe and any delimitations
                * Each command are separate by index, for example:
                * pipe_command[0][0] = "ls"
                * pipe_command[0][1] = "-l"
                * 
                * pipe_command[1][0] = "wc"
                *
                ***/

                char ***pipe_command = (char ***)calloc(pipe_command_size, sizeof(char *));
                
                if (!pipe_command || errno)
                    return EOF;

                // Separate each command, without '|'
                line_2_commands = clear_array(line, "|");

                if (!line_2_commands || errno)
                    return EOF;

                for (i = 0; line_2_commands[i] != NULL; i++) {

                    // If pipe_command are smaller than the input user, realloc pipe_command
                    if (i <= pipe_command_size) {

                        pipe_command_size += BUF_SIZE;
                        pipe_command = (char ***)realloc(pipe_command, sizeof(char *) * pipe_command_size);

                        if (!pipe_command || errno)
                            return EOF;

                    }


                    check_env = clear_array(line_2_commands[i], DELIMT);

                    // Check if environnement variable
                    if (replace_env_var(check_env) == EOF || errno)
                        return EOF;

                    // Each command have their own index
                    pipe_command[i] = check_env;

                    if (!pipe_command[i] || errno)
                        return EOF;

                }

                // Set the Null Byte character in the end of the array of commands
                pipe_command[i] = NULL;
                
                if (start_pipe_processes(pipe_command) == EOF || errno)
                    return EOF;
                
                // Clear the heap
                for (i = 0; pipe_command[i] != NULL; i++)
                    free(pipe_command[i]);
                
                //Finish by clear the pipe_command
                free(pipe_command);
                    
                }

                // Start process without pipe
                else {

                    // Take off the delimitation, like spaces
                    line_clean = clear_array(line, DELIMT);                   
                    
                    if (replace_env_var(line_clean) == EOF || errno)
                        return EOF;

                    if (!line_clean || errno)
                        return EOF;

                    if (start_processes(line_clean) == EOF || errno)
                        return EOF;

                    free(line_clean);
                
                }

        }

        // Delete prompt
        free(prompt);
        // Delete the user input
        free(line);

    }


    return 0;

}
