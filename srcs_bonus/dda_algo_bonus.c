/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda_algo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/04 19:12:34 by mframbou          #+#    #+#             */
/*   Updated: 24-02-2022 17:04 by                                             */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
	Just return if every tile we should go -x or +x, same for y
*/
static t_point	get_direction_steps(t_vector direction)
{
	t_point	step;

	if (direction.x < 0)
		step.x = -1;
	else
		step.x = 1;
	if (direction.y < 0)
		step.y = -1;
	else
		step.y = 1;
	return (step);
}

/*
	Gets the first DDA distance, this is the only one which we should
	pre-calculate, because if the player is in the middle of a tile,
	the distance will not be the same as for tile to tile (x to x+1 or x to y+1)

	The player relative position is the position within the tile (between 0-1)
*/
static t_vector	init_base_distances(t_vector direction, t_vector dda_distances, \
									t_vector player_pos)
{
	t_vector	distances;
	t_vector	player_relative_pos;

	player_relative_pos.x = (player_pos.x - (double)((int) player_pos.x));
	player_relative_pos.y = (player_pos.y - (double)((int) player_pos.y));
	if (direction.x < 0)
		distances.x = player_relative_pos.x * dda_distances.x;
	else
		distances.x = (1.0 - player_relative_pos.x) * dda_distances.x;
	if (direction.y < 0)
		distances.y = player_relative_pos.y * dda_distances.y;
	else
		distances.y = (1.0 - player_relative_pos.y) * dda_distances.y;
	return (distances);
}

/*
	Wall pos hit = position between 0-1 where our ray hit.
	This value is used for texture projection

	fmod = float modulo (same as wall_pos_hit - floor(wall_pos_hit))
*/
t_ray_hit	get_ray_hit(t_vector direction, t_map map, t_vector player_pos)
{
	t_ray		ray;
	t_ray_hit	hit;

	ray.direction = direction;
	ray.current_tile = get_pos_current_tile(player_pos);
	ray.dda_distances = get_dda_distances(ray.direction);
	ray.direction_steps = get_direction_steps(ray.direction);
	ray.total_distances = init_base_distances(ray.direction, \
											ray.dda_distances, player_pos);
	hit = do_the_dda_algorithm(ray, map, player_pos);
	hit.direction = direction;
	if (hit.side_hit == 'x')
		hit.wall_pos_hit = player_pos.y + hit.distance * hit.direction.y;
	else
		hit.wall_pos_hit = player_pos.x + hit.distance * hit.direction.x;
	hit.wall_pos_hit = fmod(hit.wall_pos_hit, 1.0);
	return (hit);
}
