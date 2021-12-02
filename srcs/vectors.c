/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vectors.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 11:57:54 by mframbou          #+#    #+#             */
/*   Updated: 2021/11/23 21:13:37 by mframbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

double	degrees_to_radians(double angle)
{
	return (angle * (M_PI / 180.0));
}

inline double	get_x_direction(double angle)
{
	return (cos(degrees_to_radians(angle)));
}

inline double	get_y_direction(double angle)
{
	return (sin(degrees_to_radians(angle)));
}
