#include "../include/nm_otool.h"
#include <stdio.h>

void		qsort_ar(t_ofile **arr, int l, int r)
{
	int		i;
	int		j;
	t_ofile	*temp;
	char	*x;

	i = l;
	j = r;
	x = arr[(l + r) / 2]->name;
	while (i <= j)
	{
		while (ft_strcmp(arr[i]->name, x) < 0 && i < r)
			i++;
		while (ft_strcmp(arr[j]->name, x) > 0 && j > l)
			j--;
		if (i <= j && (temp = arr[i]) && (arr[i] = arr[j]) &&
				(arr[j] = temp))
		{
			i++;
			j--;
		}
	}
	if (l < j)
		qsort_ar(arr, l, j);
	if (i < r)
		qsort_ar(arr, i, r);
}

void		strtable_64_qsort(char *str, struct nlist_64 **arr, int l, int r)
{
	int				i;
	int				j;
	char			*x;
	struct nlist_64	*temp;

	i = l;
	j = r;
	x = str + arr[(l + r) / 2]->n_un.n_strx;
	while (i <= j)
	{
		while ((ft_strcmp(str + arr[i]->n_un.n_strx, x)) < 0 && i < r)
			i++;
		while ((ft_strcmp(str + arr[j]->n_un.n_strx, x)) > 0 && j > l)
			j--;
		if (i <= j && (temp = arr[i]) && (arr[i] = arr[j]) &&
				(arr[j] = temp))
		{
			i++;
			j--;
		}
	}
	if (l < j)
		strtable_64_qsort(str, arr, l, j);
	if (i < r)
		strtable_64_qsort(str, arr, i, r);
}
