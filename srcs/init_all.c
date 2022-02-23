/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 17:48:40 by mframbou          #+#    #+#             */
/*   Updated: 23-02-2022 14:38 by                                             */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
	We get 4 texture (N-S-E-W) in an array since norminette limits to 4 args
*/
int	load_textures(t_game *game, char **textures_files, int x_size, int y_size)
{
	game->n_tex.image.img = mlx_xpm_file_to_image(game->mlx, \
									textures_files[0], &x_size, &y_size);
	game->n_tex.height = y_size;
	game->n_tex.width = x_size;
	game->s_tex.image.img = mlx_xpm_file_to_image(game->mlx, \
									textures_files[1], &x_size, &y_size);
	game->s_tex.height = y_size;
	game->s_tex.width = x_size;
	game->e_tex.image.img = mlx_xpm_file_to_image(game->mlx, \
									textures_files[2], &x_size, &y_size);
	game->e_tex.height = y_size;
	game->e_tex.width = x_size;
	game->w_tex.image.img = mlx_xpm_file_to_image(game->mlx, \
									textures_files[3], &x_size, &y_size);
	game->w_tex.height = y_size;
	game->w_tex.width = x_size;
	if (!game->n_tex.image.img || !game->s_tex.image.img \
	|| !game->e_tex.image.img || !game->w_tex.image.img)
	{
		free_textures(game);
		return (-1);
	}
	return (0);
}

/*
	https://string-functions.com/hex-string.aspx

	textures images are already parsed, only init addresses
*/
int	init_mlx_images_and_textures(t_game *game)
{
	if (init_minimap(game))
	{
		free_main_win(game);
		free_main_img(game);
		return (-1);
	}
	if (init_textures_addresses(game))
	{
		free_main_win(game);
		free_main_img(game);
		free_minimap(game);
		return (-1);
	}
	return (0);
}

int	init_textures_addresses(t_game *game)
{
	char	*addr;

	game->n_tex.image.addr = mlx_get_data_addr(game->n_tex.image.img, \
				&game->n_tex.image.bpp, &game->n_tex.image.line_length, \
				&game->n_tex.image.endian);
	game->s_tex.image.addr = mlx_get_data_addr(game->s_tex.image.img, \
				&game->s_tex.image.bpp, &game->s_tex.image.line_length, \
				&game->s_tex.image.endian);
	game->e_tex.image.addr = mlx_get_data_addr(game->e_tex.image.img, \
				&game->e_tex.image.bpp, &game->e_tex.image.line_length, \
				&game->e_tex.image.endian);
	game->w_tex.image.addr = mlx_get_data_addr(game->w_tex.image.img, \
				&game->w_tex.image.bpp, &game->w_tex.image.line_length, \
				&game->w_tex.image.endian);
	if (!game->n_tex.image.addr || !game->s_tex.image.addr \
	|| !game->e_tex.image.addr || !game->w_tex.image.addr)
	{
		free_textures(game);
		free_minimap(game);
		return (-1);
	}
	return (0);
}

/*
	Initialize every pixel transparent
*/
int	init_minimap(t_game *game)
{
	int	x;
	int	y;

	game->minimap.size_px = min(game->height / MINIMAP_SIZE_RATIO, \
							game->width / MINIMAP_SIZE_RATIO);
	game->minimap.img.img = mlx_new_image(game->mlx, game->minimap.size_px, \
											game->minimap.size_px);
	game->minimap.img.addr = mlx_get_data_addr(game->minimap.img.img, \
					&game->minimap.img.bpp, &game->minimap.img.line_length, \
					&game->minimap.img.endian);
	if (!game->minimap.img.img || !game->minimap.img.addr)
	{
		free_minimap(game);
		return (-1);
	}
	x = -1;
	while (++x < game->minimap.size_px)
	{
		y = -1;
		while (++y < game->minimap.size_px)
			mlx_put_pixel_img(&game->minimap.img, x, y, 0xFF000000);
	}
	return (0);
}

int	init_main_mlx_window_img(t_game *game, int width, int height, \
							char *win_name)
{
	game->mlx = mlx_init();
	if (!game->mlx)
		return (-1);
	game->window = mlx_new_window(game->mlx, width, height, win_name);
	if (!game->window)
		return (-1);
	game->main_img.img = mlx_new_image(game->mlx, width, height);
	if (!game->main_img.img)
	{
		free_main_win(game);
		return (-1);
	}
	game->main_img.addr = mlx_get_data_addr(game->main_img.img, \
											&game->main_img.bpp, \
											&game->main_img.line_length, \
											&game->main_img.endian);
	if (!game->main_img.addr)
	{
		free_main_win(game);
		free_main_img(game);
		return (-1);
	}
	return (0);
}
