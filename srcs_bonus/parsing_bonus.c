/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 13:31:04 by mframbou          #+#    #+#             */
/*   Updated: 25-02-2022 12:36 by                                             */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

static int	is_cub(char *filename)
{
	int	len;

	len = ft_strlen(filename);
	if (len >= 4)
	{
		if (filename[len - 4] == '.' \
			&& filename[len - 3] == 'c' \
			&& filename[len - 2] == 'u' \
			&& filename[len - 1] == 'b')
		{
			return (1);
		}
	}
	print_error_plus_arg("The given file is not a .cub file.\n");
	return (0);
}

static int	are_all_values_parsed(t_game *game)
{
	int	found_error;

	found_error = 0;
	if ((unsigned int) game->ceil_color == 0xFF000000)
		print_error_if_needed("Missing ceil color\n", &found_error);
	if ((unsigned int) game->floor_color == 0xFF000000)
		print_error_if_needed("Missing ceil color\n", &found_error);
	if (game->n_tex.image.img == NULL)
		print_error_if_needed("Missing ceil color\n", &found_error);
	if (game->s_tex.image.img == NULL)
		print_error_if_needed("Missing ceil color\n", &found_error);
	if (game->e_tex.image.img == NULL)
		print_error_if_needed("Missing ceil color\n", &found_error);
	if (game->w_tex.image.img == NULL)
		print_error_if_needed("Missing ceil color\n", &found_error);
	return (!found_error);
}

/*
	Parse header, then if valid reopen file and go
	to the potential first line of map (returned by parse_header).

	We do it simply by calling GNL x times
	(while freeing it directly since we don't need the value)

	flush buffer after first close since we close in the middle of file

	since we already read the first line using gnl, we need to go back to it
*/
int	parse_cub_file(char *filename, t_game *game)
{
	int	map_start_line;
	int	fd;

	if (!is_cub(filename))
		return (-1);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (print_error_plus_arg("Cannot open the .cub file \
									(non-existent or no permission)\n"));
	map_start_line = parse_header(fd, game);
	if (map_start_line == -1 || !are_all_values_parsed(game))
	{
		close(fd);
		return (-1);
	}
	close(fd);
	flush_gnl_buffer();
	if (parse_map(filename, map_start_line, game) == -1)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}
