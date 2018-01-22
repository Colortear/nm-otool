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
#include <stdio.h>

int 	main(int ac, char **av)
{
	int				fd;
	char			*ptr;
	char			*ptr_save;
	struct stat		buf;
	uint32_t		table_len;
	struct ranlib	*ran;
	char			*table;
	uint32_t		i;
	struct ar_hdr	*ar;
	int				ex;

	i = 0;
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
	ar = (struct ar_hdr *)(ptr + SARMAG);
	ex = ft_atoi(ar->ar_name + ft_strlen(AR_EFMT1));
	table_len = *((uint32_t *)(ptr + sizeof(struct ar_hdr) + SARMAG + ex));
	table_len += sizeof(struct ar_hdr) + SARMAG + ex + sizeof(uint32_t);
	i = sizeof(struct ar_hdr) + SARMAG + ex + sizeof(uint32_t);
	int count = 0;
	while (i < table_len)
	{
		ran = (struct ranlib *)(ptr + i);
		printf("%s\n", ptr + ran->ran_off);
		i += sizeof(struct ranlib);
		printf("%i\n", ++count);
	}
	printf("%li\n", sizeof(struct ranlib));
	if (munmap(ptr, buf.st_size) < 0)
	{
		write(2, "munmap fail\n", 12);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
