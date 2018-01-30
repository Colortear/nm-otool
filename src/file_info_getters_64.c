#include "../include/nm_otool.h"
#include <stdio.h>

char	get_sect_64(t_segs_64 *seg, struct nlist_64 *info)
{
	t_segs_64			*tmp;
	uint32_t			i;
	uint32_t			total;
	struct section_64	*tmp_sect;

	tmp = seg;
	total = 0;
	while ((i = -1) && tmp)
	{
		tmp_sect = (struct section_64 *)((char *)(tmp->seg) +
				sizeof(struct segment_command_64));
		while (++i < tmp->seg->nsects && ++total < info->n_sect)
			tmp_sect = (void *)tmp_sect + sizeof(struct section_64);
		if (total == info->n_sect)
		{
			if (!ft_strcmp(tmp_sect->sectname, SECT_TEXT))
				return ('T');
			else if (!ft_strcmp(tmp_sect->sectname, SECT_BSS))
				return ('B');
			else if (!ft_strcmp(tmp_sect->sectname, SECT_DATA))
				return ('D');
		}
		tmp = tmp->next;
	}
	return ('S');
}

char	get_type_64(struct nlist_64 *info, t_segs_64 *seg)
{
	char	ret;
	uint8_t	t;

	ret = 0;
	t = info->n_type & N_TYPE;
	if (t == N_UNDF || t == N_PBUD)
		ret = 'U';
	else if (t  == N_ABS)
		ret = 'A';
	else if (t == N_INDR)
		ret = 'I';
	else if (t == N_SECT)
		ret = get_sect_64(seg, info);
	else
		ret = '-';
	if (!(info->n_type & N_EXT))
		ret = ft_tolower(ret);
	return (ret);
}

void	get_value_64(struct nlist_64 *info, char type)
{
	uint64_t	value;
	int			len;
	int			it;
	char		*tab;
	char		ret[17];

	len = hex_len_64(info->n_value);
	value = info->n_value;
	tab = "0123456789abcdef";
	if (!(it = 0) && (type == 'U' || type == '-'))
	{
		write(1, "                 ", 17);
		return ;
	}
	ret[16] = ' ';
	while (it++ < 16 - len)
		ret[it - 1] = '0';
	len = 15;
	while (value && (ret[len] = tab[value % 16]))
	{
		value /= 16;
		len--;
	}
	write(1, ret, 17);
}
