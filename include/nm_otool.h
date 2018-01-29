/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm-otool.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdebs <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/01 01:41:07 by wdebs             #+#    #+#             */
/*   Updated: 2016/10/01 01:42:29 by wdebs            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_OTOOL_H
# define NM_OTOOL_H

# include "../libft/includes/libft.h"

# include <sys/mman.h>
# include <ar.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <stdlib.h>
# include <unistd.h>
# include <mach-o/ranlib.h>

enum err {OPEN_FAIL, STAT_FAIL, MAP_FAIL, UNMAP_FAIL, FRMT_ERR, ENDIAN_ERR};

typedef struct					s_ofile
{
	struct ar_hdr				*ar;
	struct ranlib				*ran;
	size_t						name_len;
	char						*name;
	char						*data;
}								t_ofile;

typedef struct					s_segs
{
	struct segment_command_64	*seg;
	struct s_segs				*next;
}								t_segs;

void	ft_nm(char *ptr, char *path);

#endif
