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

typedef struct					s_segs_64
{
	struct segment_command_64	*seg;
	struct s_segs_64			*next;
}								t_segs_64;

typedef struct					s_segs
{
	struct segment_command		*seg;
	struct s_segs				*next;
}								t_segs;


/*
** nm.c
*/

void		ft_nm(char *ptr, char *path);
int			return_error(char *path, int err_code);

/*
** sort_64.c
*/

void		strtable_64_qsort(char *str, struct nlist_64 **arr, int l, int r);

/*
** sort.c
*/

void		qsort_ar(t_ofile **arr, int l, int r);

/*
** util.c
*/

void		print_file_name(char *name, char *path, uint32_t len);
void		free_seg_64(t_segs_64 *seg);
void		free_seg(t_segs *seg);
int			hex_len_64(uint64_t value);
int			hex_len(uint32_t value);

/*
** ar.c
*/

t_ofile		**ar_allocate_sort(char *ptr, uint32_t it, uint32_t size);
void		handle_ar(char *ptr, char *path);

/*
** file_info_getters_64.c
*/

char		get_sect_64(t_segs_64 *seg, struct nlist_64 *info);
char		get_type_64(struct nlist_64 *info, t_segs_64 *seg);
void		get_value_64(struct nlist_64 *info, char type);

/*
** file64.c
*/

void		extract_load_commands_64(char *ptr, struct symtab_command *sym,
			t_segs_64 *seg);
t_segs_64	*add_seg_64(t_segs_64 *seg, struct load_command *lc);
void		get_symtab_64(char *ptr, uint32_t ncmds);
void		handle_64(char *ptr);

#endif
