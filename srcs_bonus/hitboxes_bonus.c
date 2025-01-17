/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hitboxes.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/04 19:19:20 by mframbou          #+#    #+#             */
/*   Updated: 25-02-2022 12:36 by                                             */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

/*
	If we hit a door which is not fully opened,
	count it as a wall in terms of hitbox
*/
static int	point_intersects_wall(t_vector point, t_map map)
{
	t_point		current_tile;

	current_tile = get_pos_current_tile(point);
	if (is_door(current_tile))
	{
		if (get_door(current_tile)->closed == 0.0)
			return (0);
		else
			return (1);
	}
	else if (map.map[current_tile.y][current_tile.x] != 0)
		return (1);
	return (0);
}

/*
	Literraly a square hitbox (axis aligned) around the player
	to check for collisions
*/
int	has_intersection_with_wall(t_vector player_pos, t_map map)
{
	t_vector	bounding_box_bot_left;
	t_vector	bouding_box_bot_right;
	t_vector	bounding_box_top_left;
	t_vector	bouding_box_top_right;

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
