/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ↑↑↓↓←→←→AB_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 22:50:11 by mframbou          #+#    #+#             */
/*   Updated: 2021/12/14 13:11:21 by mframbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
	UUDDLRLRAB
	↑↑↓↓←→←→AB
*/
int	is_uuddlrlrab_complete(int code[10])
{
	return (code[0] == KEY_W && code[1] == KEY_W
		&& code[2] == KEY_S && code[3] == KEY_S
		&& code[4] == KEY_A && code[5] == KEY_D
		&& code[6] == KEY_A && code[7] == KEY_D
		&& code[8] == MOUSE_LEFT && code[9] == MOUSE_RIGHT);
}

int	check_uuddlrlrab(int keycode)
{
	static int	code[10];
	int			i;

	i = 1;
	while (i < 10)
	{
		code[i - 1] = code[i];
		i++;
	}
	code[9] = keycode;
	if (is_uuddlrlrab_complete(code))
		return (1);
	return (0);
}

void	draw_special_texture(t_draw_coords draw_coords, t_ray_hit ray_hit, \
								t_game *game)
{
	int	frame;

	frame = get_current_frame() % 61;
	if (frame <= 30)
	{
		if (ray_hit.tile_hit.x % 2 == !(ray_hit.tile_hit.y % 2))
			draw_texture(draw_coords, game->bg1, ray_hit, game);
		else
			draw_texture(draw_coords, game->bg2, ray_hit, game);
	}
	else
	{
		if (ray_hit.tile_hit.x % 2 == (ray_hit.tile_hit.y % 2))
			draw_texture(draw_coords, game->bg1, ray_hit, game);
		else
			draw_texture(draw_coords, game->bg2, ray_hit, game);
	}
}

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
void	do_render_spin(t_game *game)
{
	static double	total_angle = 0.007;
	static double	multiplier = 1.07;

	spin_player(game, &total_angle);
	reset_velocity(&game->player);
	total_angle *= multiplier;
	if (game->do_the_spin == 1 && total_angle > M_PI * 2)
	{
		game->do_the_spin = -1;
		multiplier = 0.93;
	}
	else if (game->do_the_spin == -1)
	{
		game->player.pos.x = 27.5;
		game->player.pos.y = 10.5;
		if (total_angle <= 0.007)
		{
			multiplier = 1.07;
			game->do_the_spin = 0;
			total_angle = 0.007;
			reset_velocity(&game->player);
			add_player_movements(&game->player);
		}
	}
}
