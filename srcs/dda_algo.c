/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda_algo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/04 19:12:34 by mframbou          #+#    #+#             */
/*   Updated: 2021/12/07 19:41:16 by mframbou         ###   ########.fr       */
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
static t_vector	init_base_distances(t_vector direction, t_vector dda_distances, t_vector player_pos)
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
	Here we can afford an infinite loop
	because of the subject specifying that map should be enclosed with walls.
	So the ray should always hit if we get to this step

	If we find a door, check the position the ray hit on the door
	compared to the current position of the door, if it should pass
	continue the dda loop
*/
static t_ray_hit	do_the_dda_algorithm(t_ray ray, int map[mapHeight][mapWidth], t_vector player_pos)
{
	t_ray_hit	ray_hit;

	while (1)
	{
		if (ray.total_distances.x < ray.total_distances.y)
		{
			ray.total_distances.x += ray.dda_distances.x;
			ray.current_tile.x += ray.direction_steps.x;
			ray_hit.side_hit = 'x';
		}
		else
		{
			ray.total_distances.y += ray.dda_distances.y;
			ray.current_tile.y += ray.direction_steps.y;
			ray_hit.side_hit = 'y';
		}
		if (map[ray.current_tile.y][ray.current_tile.x] != 0)
		{
			if (is_door(ray.current_tile))
			{
				t_door *door;
				door = get_door(ray.current_tile);
				if (ray_hit.side_hit == 'x')
					ray_hit.distance = ray.total_distances.x - ray.dda_distances.x;
				else
					ray_hit.distance = ray.total_distances.y - ray.dda_distances.y;
				ray_hit.tile_hit = ray.current_tile;
	
				if (ray_hit.side_hit == 'x')
					ray_hit.wall_pos_hit = player_pos.y + ray_hit.distance * ray.direction.y;
				else
					ray_hit.wall_pos_hit = player_pos.x + ray_hit.distance * ray.direction.x;
				ray_hit.wall_pos_hit = ray_hit.wall_pos_hit - floor(ray_hit.wall_pos_hit);
				if (ray_hit.distance <= 2.0)
					door->should_open = 1;
				else
					door->should_open = -1;
				if (ray_hit.wall_pos_hit <= door->closed)
					break ;
			}
			else
				break ;
		}
	}
	if (ray_hit.side_hit == 'x')
		ray_hit.distance = ray.total_distances.x - ray.dda_distances.x;
	else
		ray_hit.distance = ray.total_distances.y - ray.dda_distances.y;
	ray_hit.tile_hit = ray.current_tile;
	return (ray_hit);
}

/*
	Wall pos hit = position between 0-1 where our ray hit.
	This value is used for texture projection
*/
t_ray_hit	get_ray_hit(t_vector direction, int map[mapHeight][mapWidth], t_vector player_pos)
{
	t_ray		ray;
	t_ray_hit	ray_hit;

	ray.direction = direction;
	ray.current_tile = get_pos_current_tile(player_pos);
	ray.dda_distances = get_dda_distances(ray.direction);
	ray.direction_steps = get_direction_steps(ray.direction);
	ray.total_distances = init_base_distances(ray.direction, \
											ray.dda_distances, player_pos);
	ray_hit = do_the_dda_algorithm(ray, map, player_pos);
	ray_hit.direction = direction;
	if (ray_hit.side_hit == 'x')
		ray_hit.wall_pos_hit = player_pos.y + ray_hit.distance * ray_hit.direction.y;
	else
		ray_hit.wall_pos_hit = player_pos.x + ray_hit.distance * ray_hit.direction.x;
	ray_hit.wall_pos_hit = ray_hit.wall_pos_hit - floor(ray_hit.wall_pos_hit);
	return (ray_hit);
}
