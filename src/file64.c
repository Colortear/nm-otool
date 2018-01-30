#include "../include/nm_otool.h"

struct nlist_64	**create_nlist_64_table(struct nlist_64 *arr, uint32_t len)
{
	struct nlist_64 **ret;
	uint32_t		i;

	ret = (struct nlist_64 **)malloc(sizeof(struct nlist_64 *) * len);
	i = -1;
	while (++i < len)
		ret[i] = &arr[i];
	return (ret);
}
	
void	extract_load_commands_64
(char *ptr, struct symtab_command *sym, t_segs_64 *seg)
{
	char			*strtable;
	struct nlist_64	*arr;
	struct nlist_64	**list;
	uint32_t		i;
	char			type;

	arr = (void *)ptr + sym->symoff;
	strtable = (void *)ptr + sym->stroff;
	list = create_nlist_64_table(arr, sym->nsyms);
	strtable_64_qsort(strtable, list, 0, sym->nsyms - 1);
	i = -1;
	while (++i < sym->nsyms)
		if (!(list[i]->n_type & N_STAB))
		{
			type = get_type_64(list[i], seg);
			get_value_64(list[i], type);
			write(1, &type, 1);
			write(1, " ", 1);
			ft_putendl(strtable + list[i]->n_un.n_strx);
		}
	free_seg_64(seg);
	free(list);
}

t_segs_64	*add_seg_64(t_segs_64 *seg, struct load_command *lc)
{
	t_segs_64	*new;
	t_segs_64	*tmp;

	new = (t_segs_64 *)malloc(sizeof(t_segs_64));
	new->seg = (struct segment_command_64 *)lc;
	new->next = NULL;
	tmp = seg;
	if (!seg)
		seg = new;
	else
	{
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	return (seg);
}

void	get_symtab_64(char *ptr, uint32_t ncmds)
{
	struct symtab_command	*sym;
	t_segs_64					*seg;
	struct load_command		*lc;
	struct load_command		*save;
	uint32_t				i;

	i = -1;
	lc = (void *)ptr + sizeof(struct mach_header_64);
	save = lc;
	seg = NULL;
	while (!(sym = NULL) && ++i < ncmds)
		if (lc->cmd == LC_SEGMENT_64 && (seg = add_seg_64(seg, lc)))
			lc = (void *)lc + lc->cmdsize;
	i = -1;
	lc = save;
	while (++i < ncmds)
	{
		if (lc->cmd == LC_SYMTAB)
		{
			sym = (struct symtab_command *)lc;
			break ;
		}
		lc = (void *)lc + lc->cmdsize;
	}
	sym ? extract_load_commands_64(ptr, sym, seg) : 0;
}

void	handle_64(char *ptr)
{
	struct mach_header_64	*header;
	uint32_t				i;
	uint32_t				ncmds;

	i = 0;
	header = (struct mach_header_64 *)ptr;
	ncmds = header->ncmds;
	get_symtab_64(ptr, ncmds);
}
