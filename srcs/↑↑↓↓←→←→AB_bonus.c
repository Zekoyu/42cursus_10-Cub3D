/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ↑↑↓↓←→←→AB.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 22:50:11 by mframbou          #+#    #+#             */
/*   Updated: 2021/12/09 19:02:53 by mframbou         ###   ########.fr       */
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
