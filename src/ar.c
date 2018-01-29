#include "../include/nm_otool.h"

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
