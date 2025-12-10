#include "../inc/main.h"

static int	next_word_is_quoted(const char *line, size_t start_pos)
{
	size_t	i;
	char	quote_type;

	i = start_pos + 2;
	while (line[i] && ft_isspace((unsigned char)line[i]))
		i++;
	if (line[i] == '\'' || line[i] == '"')
	{
		quote_type = line[i];
		i++;
		while (line[i] && line[i] != quote_type)
			i++;
		if (line[i] == quote_type)
			return (1);
	}
	return (0);
}

int	compute_tokens(const char *line, t_token **tokens, size_t *i)
{
	if (line[*i] == '|')
		tok_push_helper(tokens, TOK_PIPE, i, 1);
	else if (line[*i] == '<')
	{
		if (line[*i + 1] && line[*i + 1] == '<')
		{
			if (next_word_is_quoted(line, *i))
				tok_push_helper(tokens, TOK_HEREDOC, i, 2);
			else
				tok_push_helper(tokens, TOK_HEREDOC_EXP, i, 2);
		}
		else
			tok_push_helper(tokens, TOK_REDIR_IN, i, 1);
	}
	else if (line[*i] == '>')
	{
		if (line[*i + 1] && line[*i + 1] == '>')
			tok_push_helper(tokens, TOK_REDIR_APP, i, 2);
		else
			tok_push_helper(tokens, TOK_REDIR_OUT, i, 1);
	}
	else
		return (0);
	return (1);
}

static int	push_node(char *word, t_token **tokens)
{
	t_token	*node;

	node = tok_new(TOK_WORD, word);
	if (!node)
	{
		free(word);
		free_tokens(tokens[0]);
		return (0);
	}
	tok_push(&tokens[0], &tokens[1], node);
	free(word);
	return (1);
}

static int	handle_word_token(const char *line, t_token **tokens,
				size_t *i, t_tokenize_ctx *ctx)
{
	char	*word;

	word = read_word(line, i, ctx->env_hash, ctx->exit_code);
	if (!word)
	{
		free_tokens(tokens[0]);
		return (0);
	}
	if (!push_node(word, tokens))
		return (0);
	return (1);
}

t_token	*tokenize_line(const char *line, t_env_hash *env_hash, int *exit_code)
{
	t_token			*tokens[2];
	size_t			i;
	t_tokenize_ctx	ctx;

	tokens[0] = NULL;
	tokens[1] = NULL;
	ctx.env_hash = env_hash;
	ctx.exit_code = *exit_code;
	i = 0;
	while (line[i])
	{
		while (ft_isspace((unsigned char)line[i]))
			i++;
		if (!line[i])
			break ;
		if (!compute_tokens(line, tokens, &i))
			if (!handle_word_token(line, tokens, &i, &ctx))
				return (NULL);
	}
	return (tokens[0]);
}