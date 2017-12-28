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

// ok so change quicksort to be generic and accept a void pointer and pointer function. 
// Then use this to sort the array of nlist_64. once this is done run through the list and
// print the nlist with 16 bits, type, then name
// Once this is done, integrate 32 bit and little-endian/big-endian support

char	**extract_load_commands
		(char *ptr, struct load_command *lc, struct symtab_command *sym)
{
	char			**ret;
	char			*strtable;
	struct nlist_64	*arr;
	int				i;

	arr = (void *)ptr + sym->symoff;
	strtable = (void *)ptr + sym->stroff;
	ret = malloc(sizeof(char *) * (sym->nsyms + 1));
	i = 0;
	while (i < sym->nsyms)
	{
		ret[i] = ft_strdup(strtable + arr[i].n_un.n_strx);
		i++;
	}
	ret[sym->nsyms] = 0;
	return (ret);
}

char	**get_symtab(struct mach_header_64 *h, char *ptr, int ncmds)
{
	struct symtab_command	*sym;
	struct load_command		*lc;
	char					**ret;
	int						i;

	i = 0;
	lc = (void *)ptr + sizeof(*h);
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
	ret = extract_load_commands(ptr, lc, sym);
	ft_qsort_str(ret, 0, ncmds - 1);
	return (ret);
}

void	handle_64(char *ptr)
{
	struct mach_header_64	*header;
	struct load_command		*lc;
	char					**sym;
	int						i;
	int						ncmds;

	i = 0;
	header = (struct mach_header_64 *)ptr;
	ncmds = header->ncmds;
	sym = get_symtab(header, ptr, ncmds);
	lc = (void *)ptr + sizeof(*header);
	while (sym[i])
	{
		//decide what type it is here and either print spaces or print the address
		ft_putstr("                ");
		//print type
		ft_putendl(sym[i]);
		i++;
	}
/*	while (i < ncmds)
	{
		// in this loop, check each command against the macros listed in lodaer.h and send to appropriate
		// function to handle that information.

		lc = (void *)lc + lc->cmdsize;
		i++;
	}*/
}

void	ft_nm(char *ptr)
{
	int magic_number;

    magic_number = *(int *)ptr;
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
	if ((ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
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
