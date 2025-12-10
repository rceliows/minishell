#include "../inc/main.h"

t_env_hash	*create_temp_hash_for_expansion(char **envp)
{
	t_env_hash	*hash;
	t_env_entry	*entry;
	size_t		i;

	hash = malloc(sizeof(t_env_hash));
	if (!hash)
		return (NULL);
	init_hash_table(hash);
	i = 0;
	while (envp[i])
	{
		entry = extract_envp_value(envp[i]);
		if (entry && entry->name)
			set_env_variable(hash, entry->name, entry->value);
		if (entry)
		{
			free(entry->name);
			free(entry->value);
			free(entry);
		}
		i++;
	}
	return (hash);
}

int	count_digits(int n)
{
	unsigned int	count;
	unsigned int	num;

	if (n == 0)
		return (1);
	count = 0;
	if (n < 0)
	{
		num = -n;
		count = 1;
	}
	else
		num = n;
	while (num > 0)
	{
		count++;
		num /= 10;
	}
	return (count);
}

int	extract_var_bounds(char *input, size_t pos, size_t *start, size_t *end)
{
	*start = pos + 1;
	*end = *start;
	while (input[*end] && (ft_isalnum(input[*end]) || input[*end] == '_'))
		(*end)++;
	if (*end == *start)
		return (0);
	return (1);
}

void	update_quote_state(char c, t_expand_context *ctx)
{
	if (c == '\'' && !ctx->in_double_quotes)
		ctx->in_single_quotes = !ctx->in_single_quotes;
	else if (c == '"' && !ctx->in_single_quotes)
		ctx->in_double_quotes = !ctx->in_double_quotes;
}

char	*expand_environment_variables(char *input, char **envp)
{
	return (expand_environment_variables_with_exit_code(input, envp, 0));
}