/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/04 18:18:29 by mframbou          #+#    #+#             */
/*   Updated: 2021/12/07 15:48:44 by mframbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
	This draws only the textured part of the screen vertical stripe

	Always use texture_width/height - 1 because index starts at 0 to (texture_width/height - 1)

	If the texture is bigger than the screen, just "simulate" going x (until start of screen)
	and adding a step each time, so just "move" on texture ((0 - drawstart) = steps till drawstart = 0) times
*/
static void	draw_texture(t_draw_coords draw_coords, t_texture texture, t_ray_hit ray_hit, t_game *game)
{
	int				texture_x;
	double			texture_y;
	double			y_tex_step;
	unsigned int	current_px_color;
	
	texture_x = (int) round((1.0 - ray_hit.wall_pos_hit) * (double) (texture.width - 1));
	if (ray_hit.side_hit == 'x' && ray_hit.direction.x > 0)
		texture_x = (texture.width - 1) - texture_x;
	else if (ray_hit.side_hit == 'y' && ray_hit.direction.y < 0)
		texture_x = (texture.width - 1) - texture_x;
	texture_y = 0.0;
	y_tex_step = (double) (texture.height - 1) / (double) draw_coords.line_height;
	if (draw_coords.draw_start < 0)
		texture_y += -(y_tex_step * draw_coords.draw_start);
	if (draw_coords.draw_start < 0)
	{
		draw_coords.draw_start = 0;
		draw_coords.draw_end = game->height - 1;
	}
	while (draw_coords.draw_start < draw_coords.draw_end)
	{	
		current_px_color = ((unsigned int *) texture.image.addr)[(int) texture_y * texture.width + texture_x];
		mlx_put_pixel_img(&game->main_img, draw_coords.screen_x, draw_coords.draw_start++, current_px_color);
		texture_y += y_tex_step;
	}
}

static void	draw_special_texture(t_draw_coords draw_coords, t_ray_hit ray_hit, t_game *game)
{
	int frame;
	frame = get_current_frame() % 61;
	 
	if (frame <= 30)
	{
		if (ray_hit.tile_hit.x % 2 == !(ray_hit.tile_hit.y % 2))
			draw_texture(draw_coords, game->test1, ray_hit, game);
		else
			draw_texture(draw_coords, game->test2, ray_hit, game);
	}
	else
	{
		if (ray_hit.tile_hit.x % 2 == (ray_hit.tile_hit.y % 2))
			draw_texture(draw_coords, game->test1, ray_hit, game);
		else
			draw_texture(draw_coords, game->test2, ray_hit, game);
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

static void	draw_texture_depending_on_orientation (t_draw_coords draw_coords, \
												t_ray_hit ray_hit, t_game *game)
{
	char	orientation;

	orientation = get_side_hit_orientation(ray_hit);
	if (orientation == 'N')
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
	
	draw_coords.line_height = round(((double)game->height / ray_hit.distance));
	draw_coords.draw_start = -draw_coords.line_height / 2 + game->height / 2;
    draw_coords.draw_end = draw_coords.line_height / 2 + game->height / 2;
	draw_coords.screen_x = x;
	int y = 0;
	while (y < draw_coords.draw_start)
		mlx_put_pixel_img(&game->main_img, x, y++, game->ceil_color);
	if (ray_hit.tile_hit.x > (double) 24 - 1.0 || ray_hit.tile_hit.y > (double) 24 - 1.0)
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
