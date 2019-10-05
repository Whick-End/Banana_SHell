#include "Banana_shell.h"


// Extern variable, to know if the shell continue
char shell_continue;

int banana_loop(void) {

    unsigned int i = 0;
    char **cmd_array = NULL;
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
            if (*line == 0)
                return 1;

            // Save command
            add_history(line);

            // Execute command(s)

            // If in line, we have more than 1 command
            if (strchr(line, ';')) {
                
                // Separate each command 
                cmd_array = clear_array(line, ";");
                
                if (!cmd_array || errno)
                    return EOF;

                // Execute each command
                for (i = 0; cmd_array[i] != NULL; i++) {

                    if (execute_command(cmd_array[i]) == EOF || errno)
                        return EOF;

                }

            }

            else {

                if (execute_command(line) == EOF || errno)
                    return EOF;

            }

        }

        // Delete prompt
        free(prompt);
        prompt = NULL;

        // Delete the user input
        free(line);
        line = NULL;

        // Delete commands
        free(cmd_array);
        cmd_array = NULL;

    }

    return 0;

}
