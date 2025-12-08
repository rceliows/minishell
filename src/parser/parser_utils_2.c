#include "../inc/main.h"

t_redir	*redir_new(t_redir_kind k, const char *arg)
{
	t_redir	*r;

	r = ft_calloc(1, sizeof(*r));
	if (!r)
	{
		return (NULL);
	}
	r->kind = k;
	if (arg)
		r->arg = ft_strdup(arg);
	else
		r->arg = NULL;
	return (r);
}

void	redir_push(t_redir **head, t_redir *node)
{
	t_redir	*p;

	node->next = NULL;
	if (!*head)
		*head = node;
	else
	{
		p = *head;
		while (p->next)
			p = p->next;
		p->next = node;
	}
}

t_redir_kind	get_redir_kind(t_token_type type)
{
	if (type == TOK_REDIR_IN)
		return (R_IN);
	else if (type == TOK_REDIR_OUT)
		return (R_OUT);
	else if (type == TOK_REDIR_APP)
		return (R_APP);
	else if (type == TOK_HEREDOC)
		return (R_HEREDOC);
	else if (type == TOK_HEREDOC_EXP)
		return (R_HEREDOC_EXP);
	else
		return (R_HEREDOC);
}

t_cmd	*parse_error_cleanup(t_cmd *head)
{
	free_cmds(head);
	return (NULL);
}
