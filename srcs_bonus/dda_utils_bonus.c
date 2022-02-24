/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/04 19:11:33 by mframbou          #+#    #+#             */
/*   Updated: 2021/12/09 13:28:16 by mframbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static double	get_dda_x_distance(t_vector direction)
{
	if (direction.x == 0)
		return (1E30);
	else
		return (fabs(1.0 / direction.x));
}

static double	get_dda_y_distance(t_vector direction)
{
	if (direction.y == 0)
		return (1E30);
	else
		return (fabs(1.0 / direction.y));
}

t_vector	get_dda_distances(t_vector direction)
{
	t_vector	res;

	res.x = get_dda_x_distance(direction);
	res.y = get_dda_y_distance(direction);
	return (res);
}
