#include "Banana_shell.h"


// Extern variable, to know if the shell continue
char shell_continue;

int print_banana_shell(void) {

    char *current_directory = NULL, hostname[2048];
    int status_hostname = 0;
    
    passwd *pw = NULL;
    uid_t uid = -2;

    // Get Hostname in /proc/sys/kernel/hostname
    status_hostname = gethostname(hostname, 2048);

    if (status_hostname == -1 || errno)
        return -1;

    // Get User
    uid = geteuid();
    pw = getpwuid(uid);

    if ((signed int)uid == -2 || !pw || errno)
        return -1;

    // Get current directory
    current_directory = getcwd(NULL, 0);
    
    if (!current_directory || errno)
        return -1;
    
    // Print CLI and if the current directory is home directory of user, just print ~
    (void)fprintf(stdout ,"%s%s%s:%s%s %s%s%s /> ", YELLOW, hostname, RESET, YELLOW, pw->pw_name, GREEN, 
    (strcmp(current_directory, getHomeDirectory()) == 0) ? "~" : current_directory, RESET);
    
    return 0;

}

char *get_input(void) {
    
    size_t line_size = 0;
    char *line = NULL;
    
    if (getline(&line, &line_size, stdin) == -1 || !line || errno)
        return NULL;

    return line;

}

char **clear_array(char *m_line, char *m_delimitation) {

    /***
    *
    * This function separate, each word with index,
    * like this:
    * char *command_line = "ls -l";
    * After the function:
    * char **command_line_clear = { command_line[0], command_line[1] };
    * 
    ***/

    if (!m_line || !m_delimitation)
        return NULL;

    int i, buffer_size = 256;
    char **array_of_tokens = (char **)calloc(buffer_size, sizeof(char *));
    char *token = NULL;

    if (!array_of_tokens || errno)
        return NULL;

    // Set delimitation
    token = strtok(m_line, m_delimitation);

    if (!token || errno)
        return NULL;

    for (i = 0; token != NULL; i++) {

        // Set each word and their index
        array_of_tokens[i] = token;
        // Separate with delimitation
        token = strtok(NULL, m_delimitation);

        // If the buffer_size if smaller than m_line, realloc
        if (buffer_size >= i) {

            buffer_size += 256;
            array_of_tokens = (char **)realloc(array_of_tokens, sizeof(char *) * buffer_size);

            if (!array_of_tokens || errno)
                return NULL;

        }

    }

    // Set the NULL char
    array_of_tokens[i] = NULL;

    if (errno)
        return NULL;

    return array_of_tokens;

}

char *getHomeDirectory(void) {

    passwd *pw = NULL;
    pw = getpwuid(getuid());

    if (!pw || errno)
        perror("");

     return pw->pw_dir;

}

int execute_cd_shell_command(char **m_args) {

    /***
    * 
    * Execute command cd
    * But if the command hasn't have any argument
    * Then the command cd will execute to you home directory
    * 
    ***/

    int status_chdir = 0;

    if (!m_args || !m_args[0])
        return -1;

    else if (!m_args[1]) {

        status_chdir = chdir(getHomeDirectory());

        if (status_chdir == -1 || errno)
            return -1;

        return status_chdir;

    }

    status_chdir = chdir(m_args[1]);
    
    if (status_chdir == -1 || errno)
        return -1;
    
    return status_chdir;
    
}

int wait_parent_process(pid_t m_pid) {

    if (!m_pid)
        return -1;

    pid_t wait_pid = 0;
    int status_pid = 0;

    // Wait child
    wait_pid = waitpid(m_pid, &status_pid, WUNTRACED);

    /***
    * 
    * If waitpid haven't return child pid,
    * Or if child return normally
    * Or if child was kill by signal
    * Or if errno is set
    * Return -1/  EOF
    * 
    * Else return 0
    * 
    ***/
    
    if (wait_pid != m_pid || !WIFEXITED(status_pid) || WIFSIGNALED(status_pid) || errno)
        return -1;

    return 0;

}

int start_processes(char **m_args) {

    // If m_args is null
    if (!m_args || !m_args[0])
        return -1;

    // If exit, quit the shell
    if (strcmp(m_args[0], "exit") == 0) {
    
        shell_continue = FALSE;
        return 0;

    }

    else if (strcmp(m_args[0], "cd") == 0)
        return execute_cd_shell_command(m_args);

    else if (strcmp(m_args[0], "help") == 0)
        return print_banana_help();
    
    else if (strcmp(m_args[0], "banana") == 0)
        return print_banana();

    // Else execute command
    pid_t pid = fork();

    // If fork() failed
    if (pid == -1 || errno)
        return -1;

    // Child process
    if (pid == 0) {

        if (execvp(m_args[0], m_args) == -1 || errno)
            perror(m_args[0]);

        exit(0);

    }

    //Wait Child
    return wait_parent_process(pid);

}

int start_pipe_processes(char ***m_pipe_command) {

    if (!m_pipe_command || !*m_pipe_command[0])
      return -1;

    int save_stdin_of_last_command, fd[2] = {0};
    pid_t pid;

    /***
    * 
    * Here, pipe is start with fd
    * fd[0] = input
    * fd[1] = output
    * 
    * This function create either 1 child process or more,
    * to execute the commands
    * 
    * Pipe transformed the stdout of the first command
    * To the stdin of the second command
    * And the last command print the stdout
    * 
    * 
    ***/

    for (save_stdin_of_last_command = 0; *m_pipe_command != NULL; m_pipe_command++) {

        if (pipe(fd) == -1 || errno)
            return -1;

        pid = fork();

        if (pid == -1 || errno)
            return -1;

        // Child
        if (pid == 0) {
            
            // We use just save_stdin_of_last_command, not the fd[0]
            if (close(fd[0]) == -1 || errno)
                return -1;

            if (dup2(save_stdin_of_last_command, STDIN_FILENO) == -1 || errno)
                return -1;
            
            // If it's the last command, keep stdout
            if (*(m_pipe_command + 1) != NULL) {

                if (dup2(fd[1], STDOUT_FILENO) == -1 || errno)
                    return -1;
            
            }

            /***
            * 
            * Close the fd[1] / Output,
            * Because either stdout egal fd[1],
            * Or if it is the last command of pipe
            * We use just stdout, not fd[1]
            * 
            ***/
            
            if (close(fd[1]) == -1 || errno)
                return -1;

            if (execvp(*(m_pipe_command)[0], *m_pipe_command) == -1 || errno)
                return -1;

            //If the execvp failed and the return are not egal to -1,
            //We return -1, because normally execvp exit after execute command 
            return -1;
        
        }

        // Parent process
        wait_parent_process(pid);

        // We use just stdout, stdin and fd[0], not the fd[1]
        if (close(fd[1]) == -1 || errno)
            return -1;

        // Save the stdout of the last command
        // last stdout = new stdin
        save_stdin_of_last_command = fd[0];
        
    }

    return 0;

}

void handler(int m_number) {

    // Control-C Handler 
    //Just print newline and print the shell interface
    (void)m_number;
    (void)putc('\n', stdout);
    (void)print_banana_shell();
    (void)fflush(stdout);
    (void)signal(SIGINT, handler);

}
