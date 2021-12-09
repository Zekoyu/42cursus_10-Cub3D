/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 18:17:05 by mframbou          #+#    #+#             */
/*   Updated: 2021/12/09 19:11:08 by mframbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
	Just rotate the player of given angle
*/
static void	spin_player(t_game *game, double *angle)
{
	double			current_cos;
	double			current_sin;
	t_vector		player_dir;

	current_cos = cos(*angle);
	current_sin = sin(*angle);
	player_dir.x = game->player.direction.x;
	player_dir.y = game->player.direction.y;
	game->player.direction.x = (player_dir.x * current_cos - \
								player_dir.y * current_sin);
	game->player.direction.y = (player_dir.x * current_sin + \
								player_dir.y * current_cos);
	game->player.cam_plane.x = -game->player.direction.y;
	game->player.cam_plane.y = game->player.direction.x;
}

/*
	Spin until the player did a 720 (2*PI)
	Then slowly stop spinning and teleport

	Once we reach an almost stopped point,
		put back the player movements
*/
static void	do_render_spin(t_game *game)
{
	static double	total_angle = 0.007;
	static double	multiplier = 1.07;

	spin_player(game, &total_angle);
	reset_velocity(&game->player);
	total_angle *= multiplier;
	if (game->dqwdqwdqwd == 1 && total_angle > M_PI * 2)
	{
		game->dqwdqwdqwd = -1;
		multiplier = 0.93;
	}
	else if (game->dqwdqwdqwd == -1)
	{
		game->player.pos.x = 27.5;
		game->player.pos.y = 10.5;
		if (total_angle <= 0.007)
		{
			multiplier = 1.07;
			game->dqwdqwdqwd = 0;
			total_angle = 0.007;
			reset_velocity(&game->player);
			add_player_movements(&game->player);
		}
	}
}

static void	display_render(t_game *game)
{
	mlx_clear_window(game->mlx, game->window);
	mlx_put_image_to_window(game->mlx, game->window, \
							game->main_img.img, 0, 0);
	mlx_put_image_to_window(game->mlx, game->window, \
							game->minimap.img.img, 0, 0);
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
	if (game->dqwdqwdqwd != 0)
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
