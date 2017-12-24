#include <stdio.h>
#include <sys/mman.h>
//#include <mach-o/loader.h>
//#include <mach-o/nlist.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>

void	nm(char *ptr)
{
	;
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
	nm(ptr);
	if (munmap(ptr, buf.st_size) < 0)
	{
		fprintf(stderr, "munmap fail\n");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
