#include "../include/nm_otool.h"
#include <stdio.h>

void	print_file_name(char *name, char *path, uint32_t len)
{
	write(1, "\n", 1);
	ft_putstr(path);
	write(1, "(", 1);
	len ? write(1, name, len) : write(1, name, 16);
	write(1, "):\n", 3);
}

void	free_seg_64(t_segs_64 *seg)
{
	t_segs_64	*tmp;
	while (seg)
	{
		tmp = seg;
		seg = seg->next;
		free(tmp);
	}
}

void	free_seg(t_segs *seg)
{
	t_segs	*tmp;
	while (seg)
	{
		tmp = seg;
		seg = seg->next;
		free(tmp);
	}
}

int		hex_len_64(uint64_t value)
{
	int	i;

	i = 0;
	while (value)
	{
		value = value / 16;
		i++;
	}
	return (i);
}

int		hex_len(uint32_t value)
{
	int	i;

	i = 0;
	while (value > 7)
	{
		value = value / 8;
		i++;
	}
	return (i);
}
