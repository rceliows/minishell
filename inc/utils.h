#ifndef UTILS_H
# define UTILS_H

/* Memory management utilities */
void	free_string_array(char **array, int num_cmds);
void	free_int_array(int **array, int size);
void	cleanup_data(t_pipex *data);
void	cleanup_data_with_pids(t_pipex *data, pid_t *pids);
size_t	ft_arrlen(char **s);

/* Error handling */
void	write_atomic_error(const char *prefix,
			const char *msg, const char *suffix);
void	error_exit(char *msg);
void	strderr_exit(char *msg, t_pipex *data);

/* Pipe-specific error handling */
void	command_not_found(char *cmd, t_pipex *data, pid_t *pids);
void	file_not_accessible(char *file, t_pipex *data, pid_t *pids);
void	file_not_existing(char *file, t_pipex *data, pid_t *pids);
void	fork_fail(t_pipex *data, pid_t pid, pid_t *pids);
void	command_null(t_pipex *data, pid_t *pids);

/* String utilities */
char	*safe_strjoin_free(char *s1, const char *s2);
int		cleanup_and_exit(char *prompt, int exit_code);

/* Process management utilities */
pid_t	*allocate_pids_array(t_pipex *data);

/* Free memory utils*/
void	free_split(char **arr);
void	free_tokens(t_token *t);
void	free_cmds(t_cmd *c);

#endif
