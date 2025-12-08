#ifndef LEXER_H

# define LEXER_H
typedef enum e_token_type
{
	TOK_WORD,
	TOK_PIPE,
	TOK_REDIR_IN,
	TOK_REDIR_OUT,
	TOK_REDIR_APP,
	TOK_HEREDOC,
	TOK_HEREDOC_EXP,
}	t_token_type;

typedef struct s_token
{
	t_token_type		type;
	char				*val;
	struct s_token		*next;
}	t_token;

typedef enum e_redir_kind
{
	R_IN,
	R_OUT,
	R_APP,
	R_HEREDOC,
	R_HEREDOC_EXP,
}	t_redir_kind;

typedef struct s_redir
{
	t_redir_kind	kind;
	char			*arg;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			**argv;
	t_redir			*redir;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_variable
{
	const char		*s;
	size_t			*i;
	t_env_hash		*env_hash;
	int				exit_code;
}	t_variable;

typedef struct s_tokenize_ctx
{
	t_env_hash	*env_hash;
	int			exit_code;
}	t_tokenize_ctx;

t_token	*tokenize_line(const char *line, t_env_hash *env_hash, int *exit_code);
char	*compute_expansion(t_variable *var, char *buf,
			size_t *len, size_t *cap);
char	*expand_single_variable(const char *s, size_t *i,
			t_env_hash *env_hash, int exit_code);
int		handle_quoted_content(t_variable *var, char **buf,
			size_t *len, size_t *cap);
int		handle_variable_expansion(t_variable *var,
			char **buf, size_t *len, size_t *cap);
int		append_char_to_buffer(char **buf, size_t *len, size_t *cap, char c);
int		is_word_delimiter(char c);
int		is_valid_char(char quote, t_variable *var);
t_token	*tok_new(t_token_type type, const char *val);
void	tok_push(t_token **head, t_token **tail, t_token *node);
void	tok_push_helper(t_token **tokens, t_token_type type,
			size_t *i, int val);
char	*read_word(const char *s, size_t *i,
			t_env_hash *env_hash, int exit_code);

#endif
