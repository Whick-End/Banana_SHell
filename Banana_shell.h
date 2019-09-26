#ifndef BANANA_SHELL
#define BANANA_SHELL

#include <readline/readline.h>
#include <readline/history.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/wait.h>
#include <err.h>
#include <pwd.h>

// Delimtation to clean array
#define DELIMT " \t\n\r"
// Clear screen
#define CLEAR_SHELL printf("\033[H\033[2J");

#define TRUE 1
#define FALSE 0
#define BUF_SIZE 256
#define HOST_BUF 2048

// Colors
#define DARK_BLUE "\e[38;5;45m"
#define GREEN "\e[38;5;118m"
#define YELLOW "\e[38;5;226m"
#define LIGHT_YELLOW "\e[38;5;220m"
#define BROWN "\e[38;5;130m"
#define BLUE "\e[38;5;110m"
#define RESET "\e[37m"

// Readline colors
#ifndef __APPLE__
  #define RL_GREEN "\001\e[38;5;118m\002"
  #define RL_YELLOW "\001\e[38;5;226m\002"
#endif

#define RL_RESET "\001\e[0m\002"

typedef struct passwd passwd;
extern char shell_continue;

// Main of Banana Shell
int banana_shell(void);

// Loop of Banana Shell
int banana_loop(void);

// Get the Banana Shell
char *get_banana_shell(void);

// Print Banana SHell and get user input
char *get_input(char *m_cli);

// Clear array for execution
char **clear_array(char *m_line, char *m_delimitation);

// Run command(s)
int execute_command(char *m_line);

// Create a new process to start command input
int start_processes(char **m_args);

// Create a new process to start commands input with PIPE
int start_pipe_processes(char ***m_pipe_command);

// Function to wait child in safety
int wait_parent_process(pid_t m_pid);

// Change Directory
int execute_cd_shell_command(char **m_args);

// Search environnement variable, and replace it by their value
int replace_env_var(char **m_args);

// Export environnement variable
int export(char **m_export_command);

// Delete environnement variable
int unset(char **m_unset_command);

// Get the Home Directory of User
char *getHomeDirectory(void);

// If Ctrl-C is catch, handler is call
void handler(int m_number);

// Print the menu BANANA !!
int print_banana(void);

// Print the help, if the command help is enter
int print_banana_help(void);

// Print the current version
int print_version(void);

#endif
