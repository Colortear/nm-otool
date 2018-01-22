#include "libft.h"

int		ft_strequxact(char *s1, char *s2, size_t len)
{
	size_t	i;
	int		check;

	i = 0;
	check = 0;
	while (i < len || s1[i] || s2[i])
	{
		if (s1[i] != s2[i] && (check = 1))
			break ;
		i++;
	}
	if (!check && !s1[i] && !s2[i])
		return (1);
	return (0);
}
