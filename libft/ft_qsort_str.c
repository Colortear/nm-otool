/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_qsort_str.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdebs <wdebs@student.42.us.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/07 19:11:49 by wdebs             #+#    #+#             */
/*   Updated: 2017/03/15 23:40:06 by wdebs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_qsort_str(char **arr, int l, int r)
{
	int		i;
	int		j;
	char	*temp;

	i = l;
	j = r;
	while (i <= j)
	{
		while (ft_strcmp(arr[i], arr[(l + r) / 2]) < 0 && i < r)
			i++;
		while (ft_strcmp(arr[j], arr[(l + r) / 2]) > 0 && j > l)
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
		ft_qsort_str(arr, l, j);
	if (i < r)
		ft_qsort_str(arr, i, r);
}
