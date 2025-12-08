#include "../inc/main.h"

t_token	*tok_new(t_token_type type, const char *val)
{
	t_token	*t;

	t = ft_calloc(1, sizeof(*t));
	if (!t)
		return (NULL);
	t->type = type;
	if (val)
		t->val = ft_strdup(val);
	return (t);
}

void	tok_push(t_token **head, t_token **tail, t_token *node)
{
	if (!*head)
	{
		*head = node;
		*tail = node;
	}
	else
	{
		(*tail)->next = node;
		*tail = node;
	}
}

void	tok_push_helper(t_token **tokens, t_token_type type, size_t *i, int val)
{
	tok_push(&tokens[0], &tokens[1], tok_new(type, NULL));
	*i += val;
}
