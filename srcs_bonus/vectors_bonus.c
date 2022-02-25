/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vectors.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 11:57:54 by mframbou          #+#    #+#             */
/*   Updated: 25-02-2022 12:36 by                                             */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

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

void	normalize_vector(t_vector *player_dir)
{
	double	player_dir_len;

	player_dir_len = sqrt(power_two(player_dir->x) + power_two(player_dir->y));
	player_dir->x /= player_dir_len;
	player_dir->y /= player_dir_len;
}
