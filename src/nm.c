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
/*	else if (magic_number == MH_MAGIC)
		handle_32(ptr);*/
	else if (magic_number == MH_CIGAM || magic_number == MH_CIGAM_64)
		return_error(path, ENDIAN_ERR);
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
