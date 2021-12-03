/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangles.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 23:28:57 by mframbou          #+#    #+#             */
/*   Updated: 2021/12/03 16:00:48 by mframbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int sign (t_point p1, t_point p2, t_point p3)
{
    return ((p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y));
}

int	is_point_in_triangle(t_point pt, t_point v1, t_point v2, t_point v3)
{
    int d1, d2, d3;
    int has_neg, has_pos;

    d1 = sign(pt, v1, v2);
    d2 = sign(pt, v2, v3);
    d3 = sign(pt, v3, v1);

	has_neg = 0;
	has_pos = 0;
    if ((d1 < 0) || (d2 < 0) || (d3 < 0))
		has_neg = 1;
    if ((d1 > 0) || (d2 > 0) || (d3 > 0))
		has_pos = 1;
	if (has_neg && has_pos)
		return(0);
	return (1);
}

/*
	Gets the top point (where player is pointing) of the player triangle icon
	(this function does not add the offset to center the triangle on map)
*/
t_point	get_triangle_front_point(t_vector normalized_player_dir)
{
	t_vector	current_pos;
	t_point		front_pt;
	int			i;

	i = 0;
	current_pos.x = normalized_player_dir.x;
	current_pos.y = normalized_player_dir.y;
	while (i < PLAYER_SIZE_ON_MINIMAP)
	{
		current_pos.x += normalized_player_dir.x;
		current_pos.y += normalized_player_dir.y;
		i++;
	}
	front_pt.x = (int) current_pos.x;
	front_pt.y = (int) current_pos.y;
	return (front_pt);
}

/*
	This one gets one of the side point (the other one is just the opposite of it)
	p1 = get_side_point;
	p2 = -get_side_point;

	Rotate a vector:
	x2 = x * cos(angle) - y * sin(angle)
	y2 = x * sin(angle) + y * cos(angle)

	cos(90 degrees) = 0
	sin(90 degrees) = 1

	x2 = x * 0 - y * 1
	y2 = x * 1 + y * 0

	x2 = -y
	y2 = x
*/
t_point	get_triangle_side_point(t_vector normalized_player_dir)
{
	t_vector	normalized_perpendicular_dir;
	t_vector	current_pos;
	t_point		side_pt;
	int			i;

	i = 0;
	normalized_perpendicular_dir.x = -normalized_player_dir.y;
	normalized_perpendicular_dir.y = normalized_player_dir.x;
	current_pos.x = normalized_perpendicular_dir.x;
	current_pos.y = normalized_perpendicular_dir.y;
	while (i < PLAYER_SIZE_ON_MINIMAP / 2.5)
	{
		current_pos.x += normalized_perpendicular_dir.x;
		current_pos.y += normalized_perpendicular_dir.y;
		i++;
	}
	side_pt.x = (int) current_pos.x;
	side_pt.y = (int) current_pos.y;
	return (side_pt);
}
