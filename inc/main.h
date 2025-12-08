#ifndef MAIN_H
# define MAIN_H

/* System includes */
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <errno.h>
# include <signal.h>
# include <termios.h>
# include <limits.h>
# include <readline/readline.h>
# include <readline/history.h>

/* Local includes */
# include "../libft/inc/libft.h"
# include "./environment.h"
# include "./lexer.h"
# include "./parser.h"
# include "./executor.h"
# include "./pipes.h"
# include "./utils.h"
# include "./commands.h"
# include "./builtin_commands.h"
# include "./signals.h"

/* Forward declarations for types used as pointers */
typedef struct s_pipex			t_pipex;
typedef struct s_token			t_token;
typedef struct s_cmd			t_cmd;
typedef struct s_env_hash		t_env_hash;

/* Global variable */
extern volatile sig_atomic_t	g_shell_signal;

/* Cleanup data structure for child processes */
typedef struct s_cleanup_data
{
	t_token		*tokens;
	t_cmd		*cmds;
	t_env_hash	*env_hash;
	char		*prompt;
}	t_cleanup_data;

/* Main entry point and shell loop */
int		main(int argc, char **argv, char **envp);

/* Core shell loop functions */
char	*trim_and_validate_input(char *input);
int		handle_shell_signal(int *exit_code);
int		handle_exit_command(char *input, int *exit_code);
void	cleanup_shell_iteration(char *input, char *prompt);
void	handle_signal_quit(int signo);

/* Prompt creation and display */
char	*setup_prompt_hash(char *cwd, t_env_hash *env_hash);

/* Cleanup functions for child processes */
void	cleanup_before_exit(t_cleanup_data *cleanup);

#endif
