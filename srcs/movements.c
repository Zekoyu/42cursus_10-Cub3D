/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movements.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 15:18:52 by mframbou          #+#    #+#             */
/*   Updated: 25-02-2022 12:13 by                                             */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	add_velocity(t_player *player, double x_direction, double y_direction)
{
	player->velocity.x += x_direction * MOVEMENT_FACTOR;
	player->velocity.y += y_direction * MOVEMENT_FACTOR;
}

void	remove_velocity(t_player *player, \
						double x_direction, double y_direction)
{
	player->velocity.x -= x_direction * MOVEMENT_FACTOR;
	player->velocity.y -= y_direction * MOVEMENT_FACTOR;
}

void	reset_velocity(t_player *player)
{
	player->velocity.x = 0;
	player->velocity.y = 0;
}

/*
	Automatically readjusts velocity
	(if player is going forward before rotate, he will still after)

	Don't make camera plane perpendicular to player but rotate is just as
	any vector, since player direction is normalized but cam plane is not
	if we change fov, it would cause it to reset to normalized (90 degrees)
*/
void	rotate_player(t_player *player, int direction)
{
	float	dir_x;
	float	cam_x;

	reset_velocity(player);
	dir_x = player->direction.x;
	player->direction.x = player->direction.x * COS_ROTATION \
							- player->direction.y * (SIN_ROTATION * direction);
	player->direction.y = dir_x * (SIN_ROTATION * direction) \
								+ player->direction.y * (COS_ROTATION);
	cam_x = player->cam_plane.x;
	player->cam_plane.x = player->cam_plane.x * COS_ROTATION \
							- player->cam_plane.y * (SIN_ROTATION * direction);
	player->cam_plane.y = cam_x * (SIN_ROTATION * direction) \
							+ player->cam_plane.y * COS_ROTATION;
	if (player->directions.forward == 1)
		add_velocity(player, player->direction.x, player->direction.y);
	if (player->directions.backward == 1)
		add_velocity(player, -player->direction.x, -player->direction.y);
	if (player->directions.left == 1)
		add_velocity(player, player->direction.y, -player->direction.x);
	if (player->directions.right == 1)
		add_velocity(player, -player->direction.y, player->direction.x);
}

/*
	The rotate_player function automatically readjust velocity	
*/
void	add_player_movements(t_player *player)
{
	if (player->directions.forward == 1)
		add_velocity(player, player->direction.x, player->direction.y);
	else if (player->directions.backward == 1)
		add_velocity(player, -player->direction.x, -player->direction.y);
	else if (player->directions.left == 1)
		add_velocity(player, player->direction.y, -player->direction.x);
	else if (player->directions.right == 1)
		add_velocity(player, -player->direction.y, player->direction.x);
	else if (player->directions.rotate_l == 1)
	{
		rotate_player(player, -1);
	}
	else if (player->directions.rotate_r == 1)
	{
		rotate_player(player, 1);
	}
}
