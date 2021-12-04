/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vectors.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 11:57:54 by mframbou          #+#    #+#             */
/*   Updated: 2021/12/04 19:21:02 by mframbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

double	degrees_to_radians(double angle)
{
	return (angle * (M_PI / 180.0));
}

double	get_x_direction(double angle)
{
	return (cos(degrees_to_radians(angle)));
}

double	get_y_direction(double angle)
{
	return (sin(degrees_to_radians(angle)));
}

double	get_vector_length(t_vector vector)
{
	return (sqrt(power_two(vector.x) + power_two(vector.y)));
}
