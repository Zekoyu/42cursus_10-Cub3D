/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda_algo2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 13:23:52 by mframbou          #+#    #+#             */
/*   Updated: 2021/12/09 15:32:55 by mframbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	set_distance_to_smallest_dda(t_ray *ray, t_ray_hit *ray_hit)
{
	if (ray->total_distances.x < ray->total_distances.y)
	{
		ray->total_distances.x += ray->dda_distances.x;
		ray->current_tile.x += ray->direction_steps.x;
		ray_hit->side_hit = 'x';
	}
	else
	{
		ray->total_distances.y += ray->dda_distances.y;
		ray->current_tile.y += ray->direction_steps.y;
		ray_hit->side_hit = 'y';
	}
}

/*
	If we hit a door tile, check if the ray hit an "open" space or closed
*/
static int	has_collision(t_ray ray, t_ray_hit hit, t_vector player_pos)
{
	t_point	current_tile;
	t_door	*door;

	current_tile = ray.current_tile;
	if (is_door(current_tile))
	{
		door = get_door(ray.current_tile);
		if (hit.side_hit == 'x')
		{
			hit.distance = ray.total_distances.x - ray.dda_distances.x;
			hit.wall_pos_hit = player_pos.y + hit.distance * ray.direction.y;
		}
		else
		{
			hit.distance = ray.total_distances.y - ray.dda_distances.y;
			hit.wall_pos_hit = player_pos.x + hit.distance * ray.direction.x;
		}
		hit.wall_pos_hit = hit.wall_pos_hit - floor(hit.wall_pos_hit);
		if (hit.wall_pos_hit <= door->closed)
			return (1);
		else
			return (0);
	}
	else
		return (1);
}

/*
	Here we can afford an infinite loop
	because of the subject specifying that map should be enclosed with walls.
	So the ray should always hit if we get to this step

	If we find a door, check the position the ray hit on the door
	compared to the current position of the door, if it should pass
	continue the dda loop
*/
t_ray_hit	do_the_dda_algorithm(t_ray ray, t_map map, t_vector player_pos)
{
	t_ray_hit	ray_hit;

	while (1)
	{
		set_distance_to_smallest_dda(&ray, &ray_hit);
		if (map.map[ray.current_tile.y][ray.current_tile.x] != 0 \
		|| is_door(ray.current_tile))
			if (has_collision(ray, ray_hit, player_pos))
				break ;
	}
	if (ray_hit.side_hit == 'x')
		ray_hit.distance = ray.total_distances.x - ray.dda_distances.x;
	else
		ray_hit.distance = ray.total_distances.y - ray.dda_distances.y;
	ray_hit.tile_hit = ray.current_tile;
	return (ray_hit);
}
