/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 23:09:51 by mframbou          #+#    #+#             */
/*   Updated: 2021/12/09 17:42:32 by mframbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
	Since we will put image with an offset, calculate it with an offset also
*/
static int	is_point_in_minimap_bounds(t_point px, int minimap_size)
{
	px.x += minimap_size / 2;
	px.y += minimap_size / 2;
	return (px.x >= 0 && px.x < minimap_size \
	&& px.y >= 0 && px.y < minimap_size);
}

/*
	Normalize the player direction then draw triangle

	X and Y = center of shortest side (middle of shortest side = player pos)
	Techniquo dé crasso = start from x - 1 and do ++x

	Triangle is always centered so offset will always be minimap_size / 2

	Added a check to verify that the pixel we want to write is
	not outside of the image (which would cause a segfault)
*/
static void	draw_player_pos_dir(t_vector player_dir, t_img_data *img, \
								int minimap_size, int color)
{	
	t_point		t1;
	t_point		t2;
	t_point		top;
	t_point		current_px;
	t_point		target_px;

	normalize_vector(&player_dir);
	top = get_triangle_front_point(player_dir);
	t1 = get_triangle_side_point(player_dir);
	t2.x = -t1.x;
	t2.y = -t1.y;
	current_px.x = get_min_x(t1, t2, top) - 1;
	current_px.y = get_min_y(t1, t2, top) - 1;
	target_px.x = get_max_x(t1, t2, top);
	target_px.y = get_max_y(t1, t2, top);
	while (++current_px.x <= target_px.x)
	{
		current_px.y = get_min_y(t1, t2, top) - 1;
		while (++current_px.y <= target_px.y)
			if (is_point_in_triangle(current_px, t1, t2, top))
				if (is_point_in_minimap_bounds(current_px, \
											minimap_size))
					mlx_put_pixel_img(img, current_px.x + minimap_size / 2, \
									current_px.y + minimap_size / 2, color);
	}
}

static void	put_map_px_if_needed(t_map map, t_point pixel, \
								t_point tile, t_img_data *image)
{
	if (tile.x >= 0 && tile.y >= 0 && tile.x < map.width && tile.y < map.height)
	{
		if (map.map[tile.y][tile.x] != 0)
			mlx_put_pixel_img(image, pixel.x, pixel.y, \
								MINIMAP_FOREGROUND_COLOR);
	}
}

static void	put_minimap_pixels(t_minimap *minimap, \
								double pos_offset_every_px, \
								t_vector start_pos, t_map map)
{
	t_point		current_px;
	t_point		current_tile;
	t_vector	current_pos;

	current_px.y = -1;
	current_pos.y = start_pos.y;
	while (++current_px.y < minimap->size_px)
	{
		current_px.x = -1;
		current_pos.x = start_pos.x;
		while (++current_px.x < minimap->size_px)
		{
			current_tile = get_pos_current_tile_floor(current_pos);
			mlx_put_pixel_img(&minimap->img, current_px.x, current_px.y, \
							MINIMAP_BACKGROUND_COLOR);
			put_map_px_if_needed(map, current_px, current_tile, &minimap->img);
			current_pos.x += pos_offset_every_px;
		}
		current_pos.y += pos_offset_every_px;
	}
}

/*
	If we are at pos -0, truncating will give 0, floor will give -1
	So when we were on -x and -y, walls would actually appear 2x thicccer
	That's why I use floor only for this function
	(since i think it's more computational intensive)

	Start position = (playerPos - minimap_size in game coordinates) / 2 
					(so player is centered)
	Minimap is always a square (ratio of the screen size)
*/
void	add_minimap(t_minimap *minimap, t_map map, t_player player)
{
	double		offset_every_px;
	t_vector	current_pos;
	int			minimap_size_px;

	current_pos.x = player.pos.x - \
				((double) minimap->size_px / (double) SIZE_OF_TILE_ON_MINIMAP) \
				/ 2.0;
	current_pos.y = player.pos.y - \
				((double) minimap->size_px / (double) SIZE_OF_TILE_ON_MINIMAP) \
				/ 2.0;
	offset_every_px = ((double) minimap->size_px / \
					(double) SIZE_OF_TILE_ON_MINIMAP) \
					/ (double) minimap->size_px;
	put_minimap_pixels(minimap, offset_every_px, current_pos, map);
	draw_player_pos_dir(player.direction, &minimap->img, minimap->size_px, \
						MINIMAP_PLAYER_COLOR);
}
