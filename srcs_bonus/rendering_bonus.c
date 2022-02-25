/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 18:17:05 by mframbou          #+#    #+#             */
/*   Updated: 25-02-2022 12:36 by                                             */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	do_render_spin(t_game *game);

static void	display_render(t_game *game)
{
	mlx_clear_window(game->mlx, game->window);
	mlx_put_image_to_window(game->mlx, game->window, \
							game->main_img.img, 0, 0);
	mlx_put_image_to_window(game->mlx, game->window, \
							game->minimap.img.img, 0, 0);
}

#ifdef DO_BONUSES

void	do_render(t_game *game)
{
	t_vector	ray_dir;
	t_vector	direction;
	t_ray_hit	ray_hit;
	double		camera_pos_on_plane;
	int			x;

	direction.x = game->player.direction.x;
	direction.y = game->player.direction.y;
	if (game->do_the_spin != 0)
		do_render_spin(game);
	x = 0;
	while (x < game->width)
	{
		camera_pos_on_plane = (2.0 * x) / (double) game->width - 1;
		ray_dir.x = direction.x + \
					(game->player.cam_plane.x * camera_pos_on_plane);
		ray_dir.y = direction.y + \
					(game->player.cam_plane.y * camera_pos_on_plane);
		ray_hit = get_ray_hit(ray_dir, game->map, game->player.pos);
		drawline_from_distance(x, ray_hit, game);
		x++;
	}
	add_minimap(&game->minimap, game->map, game->player);
	display_render(game);
}
#else

/*
	If player is in wall, set black screen
*/
static int	check_if_player_in_wall(t_game *game)
{
	t_point	player_tile;
	t_map	map;
	int		x;
	int		y;

	map = game->map;
	player_tile = get_pos_current_tile(game->player.pos);
	if (player_tile.x < 0 || player_tile.x >= map.width \
	|| player_tile.y < 0 || player_tile.y >= map.height \
	|| map.map[player_tile.y][player_tile.x] != 0)
	{
		x = 0;
		while (x < game->width)
		{
			y = 0;
			while (y < game->height)
			{
				mlx_put_pixel_img(&game->main_img, x, y, 0);
				y++;
			}
			x++;
		}
		return (1);
	}
	return (0);
}

void	do_render(t_game *game)
{
	t_vector	ray_dir;
	t_vector	direction;
	t_ray_hit	ray_hit;
	double		camera_pos_on_plane;
	int			x;

	direction.x = game->player.direction.x;
	direction.y = game->player.direction.y;
	x = 0;
	if (!check_if_player_in_wall(game))
	{
		while (x < game->width)
		{
			camera_pos_on_plane = (2.0 * x) / (double) game->width - 1;
			ray_dir.x = direction.x + \
						(game->player.cam_plane.x * camera_pos_on_plane);
			ray_dir.y = direction.y + \
						(game->player.cam_plane.y * camera_pos_on_plane);
			ray_hit = get_ray_hit(ray_dir, game->map, game->player.pos);
			drawline_from_distance(x, ray_hit, game);
			x++;
		}
	}
	display_render(game);
}
#endif
