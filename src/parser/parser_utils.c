#include "../inc/main.h"

static char	*strip_quotes_from_delimiter(const char *delimiter)
{
	size_t	len;

	if (!delimiter)
		return (NULL);
	len = strlen(delimiter);
	if (len < 2)
		return (ft_strdup(delimiter));
	if ((delimiter[0] == '\'' || delimiter[0] == '"')
		&& delimiter[0] == delimiter[len - 1])
		return (ft_substr(delimiter, 1, len - 2));
	return (ft_strdup(delimiter));
}

static int	no_word(void)
{
	printf("syntax error: redirection requires a word\n");
	return (-1);
}

int	handle_redir_token(t_cmd *cur, t_token *tokens)
{
	t_token			*next_token;
	t_redir_kind	k;
	t_redir			*node;
	char			*delimiter;

	next_token = tokens->next;
	if (!next_token || next_token->type != TOK_WORD)
		return (no_word());
	k = get_redir_kind(tokens->type);
	if (k == R_HEREDOC || k == R_HEREDOC_EXP)
		delimiter = strip_quotes_from_delimiter(next_token->val);
	else
		delimiter = ft_strdup(next_token->val);
	if (!delimiter)
		return (-1);
	node = redir_new(k, delimiter);
	free(delimiter);
	if (!node)
	{
		perror("redir_new");
		return (-1);
	}
	redir_push(&cur->redir, node);
	return (0);
}
