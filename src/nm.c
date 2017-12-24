#include <stdio.h>
#include <sys/mman.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>

void	print_output(int nsyms, int symoff, int stroff, char *ptr)
{
	int				i;
	char			*strtable;
	struct nlist_64	*arr;


	arr = (void *)ptr + symoff;
	strtable = (void *)ptr + stroff;
	i = 0;
	while (i < nsyms)
	{
		printf("%s\n", strtable + arr[i].n_un.n_strx);
		i++;
	}
}

void	handle_64(char *ptr)
{
	int						ncmds;
	int						i;
	struct mach_header_64	*header;
	struct load_command		*lc;
	struct symtab_command	*sym;

	header = (struct mach_header_64 *)ptr;
	ncmds = header->ncmds;
	i = 0;
	lc = (void *)ptr + sizeof(*header);
	while (i < ncmds)
	{
		if (lc->cmd == LC_SYMTAB)
		{
			sym = (struct symtab_command *)lc;
			print_output(sym->nsyms, sym->symoff, sym->stroff, ptr);
			break ;
		}
		lc = (void *)lc + lc->cmdsize;
		i++;
	}
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
		fprintf(stderr, "Please give me an arg\n");
		return (EXIT_FAILURE);
	}
	if ((fd = open(av[1], O_RDONLY)) < 0)
	{
		fprintf(stderr, "Open error\n");
		return (EXIT_FAILURE);
	}
	if (fstat(fd, &buf) < 0)
	{
		fprintf(stderr, "fstat fail\n");
		return (EXIT_FAILURE);
	}
	if ((ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
	{
		fprintf(stderr, "mmap fail\n");
		return (EXIT_FAILURE);
	}
	ft_nm(ptr);
	if (munmap(ptr, buf.st_size) < 0)
	{
		fprintf(stderr, "munmap fail\n");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
