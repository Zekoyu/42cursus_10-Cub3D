/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 23:09:51 by mframbou          #+#    #+#             */
/*   Updated: 2021/12/02 23:51:55 by mframbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static t_point	get_pos_current_tile_floor(t_vector player_pos)
{
	t_point	res;

	res.x = (int) floor(player_pos.x);
	res.y = (int) floor(player_pos.y);
	return (res);
}

/*
	X and Y = center of shortest side (middle of shortest side = player pos)
	Techniquo dé crasso = start from x - 1 and do ++x
*/
static void	draw_player_pos_dir(t_vector player_dir, t_img_data *img, t_point pos, int color)
{	
	t_point		t1;
	t_point		t2;
	t_point		top;
	t_point		current_px;
	t_point		target_px;

	double player_dir_len = sqrt(pow_two(player_dir.x) + pow_two(player_dir.y));
	player_dir.x /= player_dir_len;
	player_dir.y /= player_dir_len;
	top = get_triangle_front_point(player_dir);
	t1 = get_triangle_side_point(player_dir);
	t2.x = -t1.x;
	t2.y = -t1.y;
	current_px.x = get_min_x(t1, t2, top) - 1;
	current_px.y = get_min_y(t1, t2, top) - 1;
	target_px.x = get_max_x(t1, t2, top);
	target_px.y = get_max_y(t1, t2, top);
	while (++(current_px.x) <= target_px.x)
	{
		while (++(current_px.y) <= target_px.y)
		{
			if (is_point_in_triangle(current_px, t1, t2, top))
				mlx_put_pixel_img(img, current_px.x + pos.x, current_px.y + pos.y, color);
		}
	}
}

/*
	If we are at pos -0, truncating will give 0, floor will give -1
	So when we were on -x and -y, walls would actually appear 2x thicccer
	That's why I use floor only for this function (since i think it's more intensive)

	Instead of going back to x 0 for each minimap line (y), just go back to 0

	(before)
	→ - - - - - - -
	→ - - - - - - -
	→ - - - - - - -

	(now)
	→ - - - - - - -
	  - - - - - - ↩
	↪ - - - - - - -

	Start position = (playerPos - minimap_size in game coordinates) / 2 (so player is centered)
	Minimap is always a square (ratio of the screen size)
*/
void	add_minimap(void *minimap_img, int map[mapHeight][mapWidth], t_player player)
{
	int			minimap_size_px;
	t_vector	current_pos;
	double		offset_every_px;
	t_point		current_tile;
	t_point		curr_px;

	curr_px.x = 0;
	curr_px.y = 0;
	minimap_size_px = min(screenHeight / MINIMAP_SIZE_RATIO, screenWidth / MINIMAP_SIZE_RATIO);
	current_pos.x = player.pos.x - ((double) minimap_size_px / (double) SIZE_OF_TILE_ON_MINIMAP) / 2.0;
	current_pos.y = player.pos.y - ((double) minimap_size_px / (double) SIZE_OF_TILE_ON_MINIMAP) / 2.0;
	offset_every_px = ((double) minimap_size_px / (double) SIZE_OF_TILE_ON_MINIMAP) / (double) minimap_size_px;
	while (curr_px.y < minimap_size_px)
	{
		curr_px.x = 0;
		while (curr_px.x < minimap_size_px)
		{
			current_tile = get_pos_current_tile_floor(current_pos);
			mlx_put_pixel_img(minimap_img, curr_px.x, curr_px.y, 0x88FFFFFF);

			// TODO: Do better (replace 24 with actual "parsed" size)
			if (current_tile.x >= 0 && current_tile.y >= 0 && current_tile.x < 24 && current_tile.y < 24)
				if (map[current_tile.y][current_tile.x] != 0)
					mlx_put_pixel_img(minimap_img, curr_px.x, curr_px.y, 0x00ff00bb);
			if (curr_px.y % 2 == 0)
				current_pos.x += offset_every_px;
			else
				current_pos.x -= offset_every_px;
			curr_px.x++;
		}
		current_pos.y += offset_every_px;
		curr_px.y++;
	}

	t_point triangle_pos = {minimap_size_px / 2, minimap_size_px / 2};
	draw_player_pos_dir(player.direction, minimap_img, triangle_pos, 0x00FF8855);
}
