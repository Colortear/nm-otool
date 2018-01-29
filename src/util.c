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
