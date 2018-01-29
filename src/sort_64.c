#include "../include/nm_otool.h"

static int	nlist_64_cmp(char *str, struct nlist_64 *arr, int l, int r, int i)
{
	return (ft_strcmp(str + arr[i].n_un.n_strx,
				str + arr[(l + r) / 2].n_un.n_strx));
}

void		strtable_64_qsort(char *str, struct nlist_64 *arr, int l, int r)
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
