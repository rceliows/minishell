#include "../inc/main.h"

void	free_tokens(t_token *t)
{
	t_token	*n;

	while (t)
	{
		n = t->next;
		free(t->val);
		free(t);
		t = n;
	}
}

static void	free_redirections(t_redir *r)
{
	t_redir	*rn;

	while (r)
	{
		rn = r->next;
		free(r->arg);
		free(r);
		r = rn;
	}
}

static void	free_argv_array(char **argv)
{
	int	i;

	if (argv)
	{
		i = 0;
		while (argv[i])
			free(argv[i++]);
		free(argv);
	}
}

void	free_cmds(t_cmd *c)
{
	t_cmd	*n;

	while (c)
	{
		n = c->next;
		free_argv_array(c->argv);
		free_redirections(c->redir);
		free(c);
		c = n;
	}
}
