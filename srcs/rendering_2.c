/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/04 18:18:29 by mframbou          #+#    #+#             */
/*   Updated: 2021/12/09 17:45:53 by mframbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
	This draws only the textured part of the screen vertical stripe

	Always use texture_width/height - 1
		because index starts at 0 to (texture_width/height - 1)

	If the texture is bigger than the screen,
		just "simulate" going x (until start of screen)
		and adding a step each time, so just "move" on
		texture ((0 - drawstart) = steps till drawstart = 0) times
*/
void	draw_texture(t_draw_coords draw_coords, t_texture tx, \
						t_ray_hit hit, t_game *game)
{
	int				texture_x;
	double			texture_y;
	double			y_tex_step;
	unsigned int	px_color;

	texture_x = (int) round((1.0 - hit.wall_pos_hit) * (double)(tx.width - 1));
	if (hit.side_hit == 'x' && hit.direction.x > 0)
		texture_x = (tx.width - 1) - texture_x;
	else if (hit.side_hit == 'y' && hit.direction.y < 0)
		texture_x = (tx.width - 1) - texture_x;
	texture_y = 0.0;
	y_tex_step = (double)(tx.height - 1) / (double)draw_coords.line_height;
	if (draw_coords.draw_start < 0)
		texture_y += -(y_tex_step * draw_coords.draw_start);
	if (draw_coords.draw_start < 0)
		draw_coords.draw_start = 0;
	if (draw_coords.draw_end >= game->height)
		draw_coords.draw_end = game->height - 1;
	while (draw_coords.draw_start < draw_coords.draw_end)
	{
		mlx_put_pixel_img(&game->main_img, draw_coords.screen_x, \
						draw_coords.draw_start++, \
						mlx_get_pixel_img(&tx.image, texture_x, texture_y));
		texture_y += y_tex_step;
	}
}

/*
	For doors, don't flip the textue if seeing from another side
	(handle need to be the same side either way)
*/
static void	draw_door_texture(t_draw_coords draw_coords, t_texture tx, \
								t_ray_hit hit, t_game *game)
{
	int				texture_x;
	double			texture_y;
	double			y_tex_step;
	unsigned int	current_px_color;
	float			door_pos;

	door_pos = 1.0 - get_door(hit.tile_hit)->closed;
	texture_x = (int) round(((hit.wall_pos_hit) + door_pos) \
				* (double)(tx.width - 1));
	texture_y = 0.0;
	y_tex_step = (double)(tx.height - 1) / (double)draw_coords.line_height;
	if (draw_coords.draw_start < 0)
		texture_y += -(y_tex_step * draw_coords.draw_start);
	if (draw_coords.draw_start < 0)
		draw_coords.draw_start = 0;
	if (draw_coords.draw_end >= game->height)
		draw_coords.draw_end = game->height - 1;
	while (draw_coords.draw_start < draw_coords.draw_end)
	{
		mlx_put_pixel_img(&game->main_img, draw_coords.screen_x, \
						draw_coords.draw_start++, \
						mlx_get_pixel_img(&tx.image, texture_x, texture_y));
		texture_y += y_tex_step;
	}
}

static char	get_side_hit_orientation(t_ray_hit ray_hit)
{
	t_vector	direction;

	direction = ray_hit.direction;
	if (ray_hit.side_hit == 'x')
	{
		if (direction.x < 0)
			return ('W');
		else
			return ('E');
	}
	else
	{
		if (direction.y < 0)
			return ('N');
		else
			return ('S');
	}
}

static void	draw_texture_depending_on_orientation(t_draw_coords draw_coords, \
											t_ray_hit ray_hit, t_game *game)
{
	char	orientation;

	orientation = get_side_hit_orientation(ray_hit);
	if (is_door(ray_hit.tile_hit))
		draw_door_texture(draw_coords, game->door, ray_hit, game);
	else if (orientation == 'N')
		draw_texture(draw_coords, game->n_tex, ray_hit, game);
	else if (orientation == 'S')
		draw_texture(draw_coords, game->s_tex, ray_hit, game);
	else if (orientation == 'E')
		draw_texture(draw_coords, game->e_tex, ray_hit, game);
	else if (orientation == 'W')
		draw_texture(draw_coords, game->w_tex, ray_hit, game);
}

void	drawline_from_distance(int x, t_ray_hit ray_hit, t_game *game)
{
	t_draw_coords	draw_coords;
	char			orientation;
	int				y;

	draw_coords.line_height = round(((double)game->height / ray_hit.distance));
	draw_coords.draw_start = -draw_coords.line_height / 2 + game->height / 2;
	draw_coords.draw_end = draw_coords.line_height / 2 + game->height / 2;
	draw_coords.screen_x = x;
	y = 0;
	while (y < draw_coords.draw_start)
		mlx_put_pixel_img(&game->main_img, x, y++, game->ceil_color);
	if (ray_hit.tile_hit.x > (double) game->map.width - 1.0 \
	|| ray_hit.tile_hit.y > (double) game->map.height - 1.0)
	{
		draw_special_texture(draw_coords, ray_hit, game);
	}
	else
	{
		draw_texture_depending_on_orientation(draw_coords, ray_hit, game);
	}
	y = draw_coords.draw_end;
	while (y < game->height)
		mlx_put_pixel_img(&game->main_img, x, y++, game->floor_color);
}
