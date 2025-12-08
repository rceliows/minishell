#ifndef ENVIRONMENT_H
# define ENVIRONMENT_H

# ifndef HASH_TABLE_SIZE
#  define HASH_TABLE_SIZE 127
# endif

# if (HASH_TABLE_SIZE < 1)
#  undef HASH_TABLE_SIZE
#  define HASH_TABLE_SIZE 127
#  warning "Invalid HASH_TABLE_SIZE provided, using default value 127"
# endif

/* Environment hash table structure */
typedef struct s_env_entry
{
	char				*name;
	char				*value;
	struct s_env_entry	*next;
}	t_env_entry;

typedef struct s_env_hash
{
	t_env_entry		*buckets[HASH_TABLE_SIZE];
	int				count;
}	t_env_hash;

/* Enviorment expansion structure */
typedef struct s_expand_context
{
	char		*input;
	t_env_hash	*env_hash;
	int			exit_code;
	int			in_single_quotes;
	int			in_double_quotes;
}	t_expand_context;

typedef struct s_var_expansion
{
	t_expand_context	*ctx;
	char				*result;
	int					*result_pos;
}	t_var_expansion;

/* Core hash table operations */
unsigned int	hash_placement(char *name);
int				set_env_variable(t_env_hash *env_hash, char *name, char *value);
char			*get_env_variable(t_env_hash *env_hash, char *name);
void			init_hash_table(t_env_hash *envp);
void			cleanup_hash_table(t_env_hash *envp);

/* Hash table utility functions */
t_env_entry		*create_new_env_entry(t_env_entry *var);
t_env_entry		*create_new_env_entry_direct(char *name, char *value);
void			add_to_bucket(t_env_hash *envp, t_env_entry *new_entry,
					int placement);
t_env_entry		*find_entry_to_remove(t_env_hash *env_hash,
					char *name, int placement, t_env_entry **prev);

/* Convert between envp array and hash table */
t_env_hash		*copy_envp(char **envp);
char			**hash_table_to_envp_array(t_env_hash *env_hash);
t_env_entry		*extract_envp_value(char *s);

/* Environment entry management */
void			envp_add_last(t_env_hash *envp, t_env_entry *var);
void			process_envp_entry(t_env_hash *env_hash, char *envp_str);
void			cleanup_entry_on_error(t_env_entry *entry);
char			**create_empty_envp_array(void);
char			*create_env_string(t_env_entry *entry);

/* Environment variable expansion */
char			*expand_environment_variables_hash(char *input,
					t_env_hash *env_hash, int exit_code);
char			*expand_environment_variables(char *input,
					char **envp);
char			*expand_environment_variables_with_exit_code(char *input,
					char **envp, int exit_code);
int				extract_var_bounds(char *input, int pos, int *start, int *end);
int				calc_var_len(t_expand_context *ctx, int pos, int *new_pos);
int				calc_total_len(t_expand_context *ctx);

/* Environment expansion utilities */
int				count_digits(int n);
t_env_hash		*create_temp_hash_for_expansion(char **envp);
int				advance_var(t_expand_context *ctx, int i);
char			*exit_code_str(int exit_code);
void			update_quote_state(char c, t_expand_context *ctx);

#endif
