#ifndef EXECUTOR_H
# define EXECUTOR_H

typedef struct s_env_hash		t_env_hash;
typedef struct s_cmd			t_cmd;
typedef struct s_token			t_token;
typedef struct s_cleanup_data	t_cleanup_data;

typedef struct s_interpreter
{
	t_cmd		*cmd;
	t_token		*tokens;
	pid_t		*pids;
	char		*prompt;
}	t_interpreter;

typedef struct s_fd
{
	int	in_fd;
	int	pipefd[2];
}	t_fd;

/* Main execution functions */
int		execute_builtin(t_cmd *cmds, char **env);
void	execute_pipeline(t_interpreter *interpreter,
			t_env_hash *env, int *error_code);

/* Command path resolution */
char	*get_command_path(char *cmd, t_env_hash *env_hash);

/* Redirection handling */
void	apply_redirections(t_interpreter *it, t_cleanup_data *cleanup);

/* Cleanup functions */
void	cleanup_before_exit(t_cleanup_data *cleanup);

void	update_fd_for_next_command(struct s_fd *fd, t_cmd *cmd);
int		find_cmd_len(t_cmd *cmd);
void	wait_for_all_processes(pid_t *pids, int cmd_count, int *exit_code);
void	set_cleanup(t_cleanup_data *cleanup, t_interpreter *it,
			t_env_hash *env_hash);
char	*get_command_path(char *cmd, t_env_hash *env_hash);
void	error_check(int fd, t_redir *r, int std, t_cleanup_data *cleanup);
void	free_process(char **env, char *path, t_cleanup_data *cleanup);
void	fail_dup_helper(int *saved_stdin, int *saved_stdout);
void	execute_builtin_child_process(t_interpreter *it, t_fd *fd,
			t_env_hash *env_hash, t_cleanup_data *cleanup);
int		handle_single_builtin(t_interpreter *it, t_env_hash *env_hash,
			t_cleanup_data *cleanup, int *exit_code);
void	execute_child_process(t_interpreter *it, t_fd *fd,
			t_env_hash *env_hash, t_cleanup_data *cleanup);
void	heredoc_child_process(int write_fd, t_interpreter *it,
			t_cleanup_data *cleanup);
void	heredoc_child_process_expanded(int write_fd, t_interpreter *it,
			t_cleanup_data *cleanup);
void	setup_heredoc_terminal(void);

#endif
