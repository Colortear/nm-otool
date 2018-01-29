/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdebs <wdebs@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/07 19:11:49 by wdebs             #+#    #+#             */
/*   Updated: 2017/03/15 23:40:06 by wdebs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/nm_otool.h"

int		ar_cmp(t_ofile **arr, int l, int r, int i)
{
	return (ft_strcmp(arr[i]->name, arr[(l + r) / 2]->name));
}

void	qsort_ar(t_ofile **arr, int l, int r)
{
	int		i;
	int		j;
	t_ofile	*temp;

	i = l;
	j = r;
	while (i <= j)
	{
		while (ft_strcmp(arr[i]->name, arr[(l + r) / 2]->name) < 0 && i < r)
			i++;
		while (ft_strcmp(arr[j]->name, arr[(l + r) / 2]->name) > 0 && j > l)
			j--;
		if (i <= j)
		{
			temp = arr[i];
			arr[i] = arr[j];
			arr[j] = temp;
			i++;
			j--;
		}
	}
	if (l < j)
		qsort_ar(arr, l, j);
	if (i < r)
		qsort_ar(arr, i, r);
}

void	print_file_name(char *name, char *path, uint32_t len)
{
	write(1, "\n", 1);
	ft_putstr(path);
	write(1, "(", 1);
	len ? write(1, name, len) : write(1, name, 16);
	write(1, "):\n", 3);
}

t_ofile	**ar_allocate_sort(char *ptr, uint32_t it, uint32_t size)
{
	t_ofile		**ret;
	size_t		i;
	int			len;

	i = 0;
	len = (size - it) / sizeof(struct ranlib);
	ret = (t_ofile **)malloc(sizeof(t_ofile *) * len);
	while (it < size)
	{
		ret[i] = (t_ofile *)malloc(sizeof(t_ofile));
		ret[i]->ran = (struct ranlib *)(ptr + it);
		ret[i]->ar = (struct ar_hdr *)(ptr + ret[i]->ran->ran_off);
		ret[i]->name_len = (!ft_strncmp(ret[i]->ar->ar_name, AR_EFMT1, 3))
			? ft_atoi(ret[i]->ar->ar_name + 3) : 16;
		ret[i]->name = ptr + ret[i]->ran->ran_off + sizeof(struct ar_hdr);
		ret[i]->data = ret[i]->name + ret[i]->name_len;
		it += sizeof(struct ranlib);
		i++;
	}
	qsort_ar(ret, 0, len - 1);
	return (ret);
}

void	handle_ar(char *ptr, char *path)
{
	t_ofile			**ofile_list;
	struct ar_hdr	*ar;
	uint32_t		size;
	uint32_t		i;

	ar = (struct ar_hdr *)(ptr + SARMAG);
	size = (!ft_strncmp(ar->ar_name, AR_EFMT1, 3)) ?
		ft_atoi(ar->ar_name + 3) : 0;
	i = sizeof(struct ar_hdr) + SARMAG + size + sizeof(uint32_t);
	size = i + (*((uint32_t *)(ptr + sizeof(struct ar_hdr) + SARMAG + size)));
	ofile_list = ar_allocate_sort(ptr, i, size);
	size = (size - i) / sizeof(struct ranlib);
	i = 0;
	while (i < size)
	{
		print_file_name(ofile_list[i]->name, path, ofile_list[i]->name_len);
		ft_nm(ofile_list[i]->data, NULL);
		free(ofile_list[i]);
		i++;
	}
	free(ofile_list);
}

int		nlist_64_cmp(char *str, struct nlist_64 *arr, int l, int r, int i)
{
	return (ft_strcmp(str + arr[i].n_un.n_strx,
				str + arr[(l + r) / 2].n_un.n_strx));
}

void	strtable_64_qsort(char *str, struct nlist_64 *arr, int l, int r)
{
	int				i;
	int				j;
	struct nlist_64	temp;

	i = l;
	j = r;
	while (i <= j)
	{
		while (nlist_64_cmp(str, arr, l, r, i) < 0 && i < r)
			i++;
		while (nlist_64_cmp(str, arr, l, r, j) > 0 && j > l)
			j--;
		if (i <= j)
		{
			temp = arr[i];
			arr[i] = arr[j];
			arr[j] = temp;
			i++;
			j--;
		}
	}
	if (l < j)
		strtable_64_qsort(str, arr, l, j);
	if (i < r)
		strtable_64_qsort(str, arr, i, r);
}

char	get_sect(t_segs *seg, struct nlist_64 info)
{
	t_segs				*tmp;
	uint32_t			i;
	uint32_t			total;
	struct section_64	*tmp_sect;

	tmp = seg;
	total = 0;
	while ((i = -1) && tmp)
	{
		tmp_sect = (struct section_64 *)((char *)(tmp->seg) +
				sizeof(struct segment_command_64));
		while (++i < tmp->seg->nsects && ++total < info.n_sect)
			tmp_sect = (void *)tmp_sect + sizeof(struct section_64);
		if (total == info.n_sect)
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

char	get_type(struct nlist_64 info, t_segs *seg)
{
	char	ret;
	uint8_t	t;

	ret = 0;
	t = info.n_type & N_TYPE;
	if (t == N_UNDF || t == N_PBUD)
		ret = 'U';
	else if (t  == N_ABS)
		ret = 'A';
	else if (t == N_INDR)
		ret = 'I';
	else if (t == N_SECT)
		ret = get_sect(seg, info);
	else
		ret = '-';
	return (ret);
}

int		hex_len(uint64_t value)
{
	int	i;

	i = 0;
	while (value > 15)
	{
		value = value / 16;
		i++;
	}
	return (i);
}

void	get_value_64(struct nlist_64 info, char type)
{
	uint64_t	value;
	int			len;
	int			it;
	char		*tab;
	char		ret[17];

	len = hex_len(info.n_value);
	value = info.n_value;
	tab = "0123456789abcdef";
	if (!(it = 0) && (type == 'U' || type == '-'))
	{
		write(1, "                 ", 17);
		return ;
	}
	ret[16] = ' ';
	while (it++ < 16 - len)
		ret[it] = '0';
	len = 15;
	while (value && (ret[len] = tab[value % 16]))
	{
		value /= 16;
		len--;
	}
	write(1, ret, 17);
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

	void	extract_load_commands
(char *ptr, struct symtab_command *sym, t_segs *seg)
{
	char			*strtable;
	struct nlist_64	*arr;
	uint32_t		i;
	char			type;

	arr = (void *)ptr + sym->symoff;
	strtable = (void *)ptr + sym->stroff;
	i = -1;
	strtable_64_qsort(strtable, arr, 0, sym->nsyms - 1);
	while (++i < sym->nsyms)
		if (!(arr[i].n_type & N_STAB))
		{
			type = get_type(arr[i], seg);
			get_value_64(arr[i], type);
			write(1, &type, 1);
			write(1, " ", 1);
			ft_putendl(strtable + arr[i].n_un.n_strx);
		}
	free_seg(seg);
}

t_segs	*add_seg(t_segs *seg, struct load_command *lc)
{
	t_segs	*new;
	t_segs	*tmp;

	new = (t_segs *)malloc(sizeof(t_segs));
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

void	get_symtab(char *ptr, uint32_t ncmds)
{
	struct symtab_command	*sym;
	t_segs					*seg;
	struct load_command		*lc;
	struct load_command		*save;
	uint32_t				i;

	i = -1;
	lc = (void *)ptr + sizeof(struct mach_header_64);
	save = lc;
	seg = NULL;
	while (!(sym = NULL) && ++i < ncmds)
		if (lc->cmd == LC_SEGMENT_64 && (seg = add_seg(seg, lc)))
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
	sym ? extract_load_commands(ptr, sym, seg) : 0;
}

void	handle_64(char *ptr)
{
	struct mach_header_64	*header;
	uint32_t				i;
	uint32_t				ncmds;

	i = 0;
	header = (struct mach_header_64 *)ptr;
	ncmds = header->ncmds;
	get_symtab(ptr, ncmds);
}

void	ft_nm(char *ptr, char *path)
{
	uint32_t	magic_number;

	magic_number = *(uint32_t *)ptr;
	/*	if (magic_number == FAT_MAGIC || magic_number == FAT_CIGAM)
		handle_fat(ptr, path);*/
	if (path && !ft_strncmp(ptr, ARMAG, SARMAG))
		handle_ar(ptr, path);
	else if (magic_number == MH_MAGIC_64)
		handle_64(ptr);
	/*else if (magic_number == MH_MAGIC)
	  handle_32(ptr);
	  else if (magic_number == MH_CIGAM || magic_number == MH_CIGAM_64)
	  return_error(path, ENDIAN_ERR);*/
}

int		return_error(char *path, int err_code)
{
	write(2, "ft_nm: ", 7);
	if (err_code == OPEN_FAIL)
	{
		ft_putstr_fd(path, 2);
		write(2, ": No such file or directory.\n", 29);
	}
	else if (err_code == STAT_FAIL)
		write(2, "fstat fail\n", 11);
	else if (err_code == MAP_FAIL)
		write(2, "mmap fail\n", 10);
	else if (err_code == UNMAP_FAIL)
		write(2, "munmap fail\n", 12);
	else if (err_code == ENDIAN_ERR)
		write(2, "big endian not supported\n", 25);
	else if (err_code == FRMT_ERR)
	{
		ft_putstr_fd(path, 2);
		write(2, ": The file was not recognized as a valid object file\n\n", 54);
	}
	return (EXIT_FAILURE);
}

int 	main(int argc, char **argv)
{
	int			fd;
	int			i;
	char		*ptr;
	struct stat	buf;

	i = 1;
	if (argc < 2 && !(i = 0))
		argv[0] = "a.out";
	while (i < argc)
	{
		if ((fd = open(argv[i], O_RDONLY)) < 0)
			return (return_error(argv[i], OPEN_FAIL));
		if (fstat(fd, &buf) < 0)
			return (return_error(NULL, STAT_FAIL));
		if ((ptr = mmap(0, buf.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE,
						fd, 0)) == MAP_FAILED)
			return (return_error(NULL, MAP_FAIL));
		ft_nm(ptr, argv[i]);
		if (munmap(ptr, buf.st_size) < 0)
			return (return_error(NULL, UNMAP_FAIL));
		i++;
	}
	return (EXIT_SUCCESS);
}
