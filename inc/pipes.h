#ifndef PIPES_H
# define PIPES_H

typedef struct s_pipex
{
	int			infile;
	int			outfile;
	int			**pipes;
	char		***cmds;
	char		**cmd_paths;
	int			**cmd_is_exec;
	char		*infile_name;
	char		*outfile_name;
	char		**envp;
	t_env_hash	*env_hash;
	int			num_commands;
	int			is_here_doc;
	int			is_append;
	int			exit_code;
}	t_pipex;

/* Main pipex execution */
int		pipex_execution_hash(int argc, char **argv,
			t_env_hash *env_hash, int *exit_code);

/* Pipeline execution */
void	execute_multiple_pipelines_hash(t_pipex *data, int *exit_code);

/* Pipe setup and command routing */
void	setup_first_command_hash(t_pipex *data, pid_t *pids);
void	setup_middle_command_hash(t_pipex *data, int cmd_index, pid_t *pids);
void	setup_last_command_hash(t_pipex *data, pid_t *pids);
void	execute_command_hash(t_pipex *data, int cmd_index, pid_t *pids);
void	setup_first_command_input(t_pipex *data, pid_t *pids);
void	setup_last_command_output(t_pipex *data);

/* Pipe setup utilities */
void	setup_infile(t_pipex *data, pid_t *pids);
void	close_unused_pipes(t_pipex *data, int cmd_index);

/* Data structure initialization */
void	init_data_hash(t_pipex *data, int argc,
			char **argv, t_env_hash *env_hash);
void	initialize_data_fields_hash(t_pipex *data, int argc,
			char **argv, t_env_hash *env_hash);
void	init_here_doc_data_hash(t_pipex *data, int argc,
			char **argv, t_env_hash *env_hash);
int		allocate_arrays(t_pipex *data);

/* Data initialization utilities */
void	setup_command(char **cmd, char **cmd_path, int *is_exec, char **envp);
int		allocate_cmd_exec_array(t_pipex *data);
void	initialize_array_pointers(t_pipex *data);

/* Pipe allocation and management */
int		allocate_pipes(t_pipex *data);

/* Here document implementation */
void	setup_here_doc(t_pipex *data, char *limiter);
int		is_here_doc(char *first_arg);

/* Pipe utilities */
int		count_pipes(char *input);

#endif