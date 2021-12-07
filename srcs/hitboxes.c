/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hitboxes.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/04 19:19:20 by mframbou          #+#    #+#             */
/*   Updated: 2021/12/07 18:36:07 by mframbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
	If we hit a door which is not fully opened, count it as a wall in terms of hitbox
*/
static int	point_intersects_wall(t_vector point, int map[mapHeight][mapWidth])
{
	t_point		current_tile;
	t_vector	rel_point;

	current_tile = get_pos_current_tile(point);
	if (map[current_tile.y][current_tile.x] != 0)
	{
		if (is_door(current_tile) && get_door(current_tile)->closed == 0.0)
			return (0);
		return (1);
	}
	return (0);
}

/*
	Literraly a square hitbox (axis aligned) around the player
	to check for collisions
*/
int	has_intersection_with_wall(t_vector player_pos, int map[mapHeight][mapWidth])
{
	t_vector	bounding_box_bot_left;
	t_vector	bouding_box_bot_right;
	t_vector	bounding_box_top_left;
	t_vector	bouding_box_top_right;
	t_point		current_tile;

	bounding_box_bot_left.x = player_pos.x - BOUNDING_BOX_SIDE_SIZE / 2;
	bounding_box_bot_left.y = player_pos.y - BOUNDING_BOX_SIDE_SIZE / 2;
	bouding_box_bot_right.x = player_pos.x + BOUNDING_BOX_SIDE_SIZE / 2;
	bouding_box_bot_right.y = player_pos.y - BOUNDING_BOX_SIDE_SIZE / 2;
	bouding_box_top_right.x = player_pos.x + BOUNDING_BOX_SIDE_SIZE / 2;
	bouding_box_top_right.y = player_pos.y + BOUNDING_BOX_SIDE_SIZE / 2;
	bounding_box_top_left.x = player_pos.x - BOUNDING_BOX_SIDE_SIZE / 2;
	bounding_box_top_left.y = player_pos.y + BOUNDING_BOX_SIDE_SIZE / 2;
	
	if (point_intersects_wall(bounding_box_top_left, map) \
		|| point_intersects_wall(bouding_box_top_right, map) \
		|| point_intersects_wall(bounding_box_bot_left, map) \
		|| point_intersects_wall(bouding_box_bot_right, map))
	{
		return (1);
	}
	return (0);
}
