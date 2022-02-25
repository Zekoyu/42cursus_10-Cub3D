/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coordinates_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/04 19:13:17 by mframbou          #+#    #+#             */
/*   Updated: 25-02-2022 12:36 by                                             */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

t_point	get_pos_current_tile(t_vector player_pos)
{
	t_point	res;

	res.x = (int) player_pos.x;
	res.y = (int) player_pos.y;
	return (res);
}

t_point	get_pos_current_tile_floor(t_vector player_pos)
{
	t_point	res;

	res.x = (int) floor(player_pos.x);
	res.y = (int) floor(player_pos.y);
	return (res);
}
