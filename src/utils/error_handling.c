#include "../inc/main.h"

void	write_atomic_error(const char *prefix,
			const char *msg, const char *suffix)
{
	size_t	len;
	size_t	i;
	char	buffer[1024];

	len = 0;
	i = 0;
	while (prefix && prefix[i] && len < 1020)
		buffer[len++] = prefix[i++];
	i = 0;
	while (msg && msg[i] && len < 1020)
		buffer[len++] = msg[i++];
	i = 0;
	while (suffix && suffix[i] && len < 1023)
		buffer[len++] = suffix[i++];
	buffer[len] = '\0';
	write(STDERR_FILENO, buffer, len);
}