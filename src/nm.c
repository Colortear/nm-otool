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

#include "../include/nm-otool.h"

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

void	extract_load_commands
		(char *ptr, struct symtab_command *sym)
{
	char			*strtable;
	struct nlist_64	*arr;
	uint32_t		i;

	arr = (void *)ptr + sym->symoff;
	strtable = (void *)ptr + sym->stroff;
	i = 0;
	strtable_64_qsort(strtable, arr, 0, sym->nsyms - 1);
	while (i < sym->nsyms)
	{
		if (!(arr[i].n_type & N_STAB))
			ft_putendl(strtable + arr[i].n_un.n_strx);
		i++;
	}
}

void	get_symtab(struct mach_header_64 *h, char *ptr, uint32_t ncmds)
{
	struct symtab_command	*sym;
	struct load_command		*lc;
	uint32_t				i;

	i = 0;
	lc = (void *)ptr + sizeof(*h);
	sym = NULL;
	while (i < ncmds)
	{
		if (lc->cmd == LC_SYMTAB)
		{
			sym = (struct symtab_command *)lc;
			break ;
		}
		lc = (void *)lc + lc->cmdsize;
		i++;
	}
	printf("B\n");
	extract_load_commands(ptr, sym);
}

void	handle_64(char *ptr)
{
	struct mach_header_64	*header;
	uint32_t				i;
	uint32_t				ncmds;

	i = 0;
	header = (struct mach_header_64 *)ptr;
	ncmds = header->ncmds;
	get_symtab(header, ptr, ncmds);
}

void	ft_nm(char *ptr)
{
	uint32_t	magic_number;

    magic_number = *(uint32_t *)ptr;
    if (magic_number == MH_MAGIC_64)
		handle_64(ptr);
}

int 	main(int ac, char **av)
{
	int			fd;
	char		*ptr;
	struct stat	buf;

	if (ac != 2)
	{
		write(2, "Please give me an arg\n", 22);
		return (EXIT_FAILURE);
	}
	if ((fd = open(av[1], O_RDONLY)) < 0)
	{
		write(2, "Open error\n", 11);
		return (EXIT_FAILURE);
	}
	if (fstat(fd, &buf) < 0)
	{
		write(2, "fstat fail\n", 11);
		return (EXIT_FAILURE);
	}
	if ((ptr = mmap(0, buf.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
	{
		write(2, "mmap fail\n", 10);
		return (EXIT_FAILURE);
	}
	ft_nm(ptr);
	if (munmap(ptr, buf.st_size) < 0)
	{
		write(2, "munmap fail\n", 12);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
