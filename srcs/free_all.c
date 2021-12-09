/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 17:50:37 by mframbou          #+#    #+#             */
/*   Updated: 2021/12/09 17:55:40 by mframbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	free_main_win(t_game *game)
{
	if (game->window)
		mlx_destroy_window(game->mlx, game->window);
}

void	free_main_img(t_game *game)
{
	if (!game->main_img.img)
		return ;
	if (!game->main_img.addr)
	{
		mlx_destroy_window(game->mlx, game->main_img.img);
		return ;
	}
}

void	free_minimap(t_game *game)
{
	if (!game->minimap.img.img)
		return ;
	if (!game->minimap.img.addr)
	{
		mlx_destroy_image(game->mlx, game->minimap.img.img);
		return ;
	}
}

void	free_textures(t_game *game)
{
	if (game->n_tex.image.img)
		free(game->n_tex.image.img);
	if (game->s_tex.image.img)
		free(game->s_tex.image.img);
	if (game->e_tex.image.img)
		free(game->e_tex.image.img);
	if (game->w_tex.image.img)
		free(game->w_tex.image.img);
}
