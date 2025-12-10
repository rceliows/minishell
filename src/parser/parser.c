#include "../inc/main.h"

static int	argv_push(char ***argvp, int *count, const char *s)
{
	int		i;
	char	**nv;
	char	*dup;

	i = *count;
	nv = ft_realloc(*argvp, (size_t)(*count) * sizeof(char *),
				(size_t)(i + 2) * sizeof(char *));
	if (!nv)
		return (-1);
	nv[i] = NULL;
	if (s)
		dup = ft_strdup(s);
	else
		dup = ft_strdup("");
	if (!dup)
		return (-1);
	nv[i] = dup;
	*count = i + 1;
	nv[*count] = NULL;
	*argvp = nv;
	return (0);
}

static t_cmd	*handle_pipe_token(t_cmd **tail, int *argc)
{
	t_cmd	*cur;

	if (!(*tail)->argv && !(*tail)->redir)
	{
		printf("syntax error: empty command before pipe\n");
		return (NULL);
	}
	cur = ft_calloc(1, sizeof(t_cmd));
	if (!cur)
		return (NULL);
	(*tail)->next = cur;
	*tail = cur;
	*argc = 0;
	return (cur);
}

static int	validate_commands(t_cmd *head)
{
	t_cmd	*cur;

	cur = head;
	while (cur)
	{
		if (!cur->argv && !cur->redir)
		{
			printf("syntax error: empty command\n");
			return (-1);
		}
		cur = cur->next;
	}
	return (0);
}

static int	process_token(t_token **tokens,
			t_cmd **cur, t_cmd **tail, int *argc)
{
	if ((*tokens)->type == TOK_PIPE)
	{
		*cur = handle_pipe_token(tail, argc);
		if (!*cur)
			return (-1);
		return (0);
	}
	else if ((*tokens)->type >= TOK_REDIR_IN
		&& (*tokens)->type <= TOK_HEREDOC_EXP)
	{
		if (handle_redir_token(*cur, *tokens) < 0)
			return (-1);
		*tokens = (*tokens)->next;
		return (0);
	}
	else if ((*tokens)->type == TOK_WORD)
		return (argv_push(&(*cur)->argv, argc, (*tokens)->val));
	return (0);
}

t_cmd	*parse_commands(t_token *tokens)
{
	t_cmd	*head;
	t_cmd	*tail;
	t_cmd	*cur;
	int		argc;

	head = ft_calloc(1, sizeof(t_cmd));
	if (!head)
		return (NULL);
	tail = head;
	cur = head;
	argc = 0;
	while (tokens)
	{
		if (process_token(&tokens, &cur, &tail, &argc) < 0)
			return (parse_error_cleanup(head));
		tokens = tokens->next;
	}
	if (validate_commands(head) < 0)
		return (parse_error_cleanup(head));
	return (head);
}