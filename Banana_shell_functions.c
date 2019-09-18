#include "Banana_shell.h"


// Extern variable, to know if the shell continue
char shell_continue;

char *get_banana_shell(void) {

    int status_hostname = 0;
    long int size_cli = 0;
    char *current_directory = NULL, *home_directory = NULL;
    char *username = NULL, *banana_shell = NULL;
    char hostname[HOST_BUF];

    passwd *pw = NULL;
    uid_t uid = -2;

    // Get Hostname in /proc/sys/kernel/hostname
    status_hostname = gethostname(hostname, HOST_BUF);

    if (status_hostname == EOF || errno)
        return NULL;

    // Get User
    uid = geteuid();
    pw = getpwuid(uid);

    if ((signed int)uid == -2 || !pw || errno)
        return NULL;

    // Get current directory
    current_directory = getcwd(NULL, 0);
    
    // Get home directory
    home_directory = getHomeDirectory();

    // Get username
    username = pw->pw_name;

    if (!home_directory || errno)
        return NULL;

    if (!current_directory || errno)
        return NULL;
    
    if (!username || errno)
        return NULL;

    // If current directory is home directory of user, just print ~
    if (strcmp(current_directory, home_directory) == 0)
        current_directory = "~";

    // Calculate number of sizeof(char)
    size_cli += strlen(YELLOW) * 2;
    size_cli += strlen(hostname);
    size_cli += strlen(RESET) * 2;
    size_cli += strlen(username);
    size_cli += strlen(GREEN);
    size_cli += strlen(current_directory);
    
    // For litte space
    size_cli += 50;

    banana_shell = (char *)calloc(sizeof(char), size_cli);

    if (!banana_shell || errno)
        return NULL;

    // Save CLI
    (void)sprintf(banana_shell ,"%s%s%s:%s%s %s%s%s /> ", RL_YELLOW, hostname, RL_RESET, RL_YELLOW, username, RL_GREEN, current_directory, RL_RESET);

    return banana_shell;

}

char *get_input(char *m_banana_shell) {

    char *line = NULL;

    // Print prompt (m_banana_shell), and get user input into line
    if (!(line = readline(m_banana_shell)) || errno)
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

    int i, buffer_size = BUF_SIZE;
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

            buffer_size += BUF_SIZE;
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
        return NULL;

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
        return EOF;

    else if (!m_args[1]) {

        status_chdir = chdir(getHomeDirectory());

        if (status_chdir == EOF || errno)
            return EOF;

        return status_chdir;

    }

    status_chdir = chdir(m_args[1]);
    
    if (status_chdir == EOF || errno)
        return EOF;
    
    return status_chdir;
    
}

int replace_env_var(char **m_args) {

    if (!m_args)
        return EOF;

    char *env_var = NULL;
    int i;

    for (i = 0; m_args[i] != NULL; i++) {

        if (m_args[i][0] == '$') {

            env_var = getenv(m_args[i]+1);
            
            // If getenv failed or the NAME is wrong
            if (!env_var || errno) {
                
                m_args[i][0] = ' ';
                m_args[i][1] = '\0';
                return 0;

            }

            m_args[i] = env_var;

        }

    }

    return 0;

}

int export(char **m_export_command) {

    if (!m_export_command)
        return EOF;

    // If threre aren't name with export command
    if (!m_export_command[1])
        return 0;

    // Separate the name and the value
    char **clear_name_value = clear_array(m_export_command[1], "=");

    if (!clear_name_value || errno)
        return EOF;

    // clear_name_value[0] = Name, clear_name_value[1] = Value
    if (setenv(clear_name_value[0], clear_name_value[1], 1) == EOF || errno)
        return EOF;
    
    return 0;

}

int unset(char **m_unset_command) {

    if (!m_unset_command)
        return EOF;

    // If there aren't name with unset command
    if (!m_unset_command[1])
        return 0;

    // Call unsetenv with the name of the variable
    if (unsetenv(m_unset_command[1]) == EOF || errno)
        return EOF;

    return 0;

}

int wait_parent_process(pid_t m_pid) {

    if (!m_pid)
        return EOF;

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
    * Return -1/EOF
    * 
    * Else return 0
    * 
    ***/
    
    if (wait_pid != m_pid || !WIFEXITED(status_pid) || WIFSIGNALED(status_pid) || errno)
        return EOF;

    return 0;

}

int start_processes(char **m_args) {

    // If m_args is null
    if (!m_args || !m_args[0])
        return EOF;

    // If exit, quit the shell
    if (strcmp(m_args[0], "exit") == 0) {
    
        shell_continue = FALSE;
        return 0;

    }

    else if (strcmp(m_args[0], "cd") == 0)
        return execute_cd_shell_command(m_args);

    else if (strcmp(m_args[0], "export") == 0)
        return export(m_args);

    else if (strcmp(m_args[0], "unset") == 0)
        return unset(m_args);

    else if (strcmp(m_args[0], "help") == 0)
        return print_banana_help();
    
    else if (strcmp(m_args[0], "banana") == 0)
        return print_banana();

    else if (strcmp(m_args[0], "version") == 0)
        return print_version();

    // Else execute command
    pid_t pid = fork();

    // If fork() failed
    if (pid == EOF)
        return EOF;

    // Child process
    if (pid == 0) {

        // Patch to MacOS
        // https://stackoverflow.com/questions/20295011/errno-set-in-child-process-after-fork-osx
        errno = 0;

        // Normally, execvp not pop RIP register from the stack
        if (execvp(m_args[0], m_args) == EOF)
            perror(m_args[0]);

        // Often when the command are wrong
        exit(1);

    }

    //Wait Child
    return wait_parent_process(pid);

}

int start_pipe_processes(char ***m_pipe_command) {

    if (!m_pipe_command || !*m_pipe_command[0])
      return EOF;

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

        if (pipe(fd) == EOF || errno)
            return EOF;

        pid = fork();

        if (pid == EOF)
            return EOF;

        // Child
        if (pid == 0) {
            
            // Patch to MacOS
            // https://stackoverflow.com/questions/20295011/errno-set-in-child-process-after-fork-osx
            errno = 0;

            // We use just save_stdin_of_last_command, not the fd[0]
            if (close(fd[0]) == EOF || errno)
                return EOF;

            if (dup2(save_stdin_of_last_command, STDIN_FILENO) == EOF || errno)
                return EOF;
            
            // If it's the last command, keep stdout
            if (*(m_pipe_command + 1) != NULL) {

                if (dup2(fd[1], STDOUT_FILENO) == EOF || errno)
                    return EOF;
            
            }

            /***
            * 
            * Close the fd[1] / Output,
            * Because either stdout egal fd[1],
            * Or if it is the last command of pipe
            * We use just stdout, not fd[1]
            * 
            ***/
            
            if (close(fd[1]) == EOF || errno)
                return EOF;

            // Normally, execvp not pop RIP register from the stack
            if (execvp(*m_pipe_command[0], *m_pipe_command) == EOF || errno)
                perror(*m_pipe_command[0]);

            // Often when command are wrong
            exit(1);
        
        }

        // Parent process
        wait_parent_process(pid);

        // We use just stdout, stdin and fd[0], not the fd[1]
        if (close(fd[1]) == EOF || errno)
            return EOF;

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
    // Print prompt
    rl_on_new_line();
    // Clear line
    rl_replace_line("", 0);
    // Start modifications
    rl_redisplay();
    (void)fflush(stdout);
    (void)signal(SIGINT, handler);

}
