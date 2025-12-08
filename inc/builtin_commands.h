#ifndef BUILTIN_COMMANDS_H
# define BUILTIN_COMMANDS_H

/* Main builtin command dispatcher */
int		execute_builtin_hash(t_cmd *cmd,
			t_env_hash *env_hash, int *exit_code, t_cleanup_data *cleanup);
int		is_builtin_command(char *cmd);

/* Command callers */
int		execute_cd_builtin(char **split_input,
			t_env_hash *env_hash, int *exit_code);
int		execute_pwd_builtin(char **args, int *exit_code);
int		execute_echo_builtin(char **split_input,
			t_env_hash *env_hash, int *exit_code);
int		execute_env_builtin(t_env_hash *env_hash, int *exit_code);
int		execute_export_builtin(char **split_input,
			t_env_hash *env_hash, int *exit_code);

/* Change directory command */
int		execute_cd_hash(char **split_input, t_env_hash *env_hash);

/* Echo command with variable expansion */
void	execute_echo_hash(char **split_input, int *exit_code,
			t_env_hash *env_hash);

/* Environment display command */
int		execute_env_hash(t_env_hash *env_hash);

/* Export environment variables */
int		builtin_export(char **args, t_env_hash *envp, int *exit_code);
void	print_export_entry(t_env_entry *entry);
void	print_export_format(t_env_hash *env_hash);
int		is_valid_identifier(char *name);

/* Unset environment variables */
int		builtin_unset(char **args, t_env_hash *envp);

/* Print working directory */
int		execute_pwd(char **args);

/* Exit command */
int		execute_exit(char **args, int *exit_code, t_cleanup_data *cleanup);

#endif
