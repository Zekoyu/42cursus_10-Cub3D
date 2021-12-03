/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movements.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 15:18:52 by mframbou          #+#    #+#             */
/*   Updated: 2021/12/03 15:20:06 by mframbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	add_velocity(t_player *player, double x_direction, double y_direction)
{
	player->velocity.x += x_direction * MOVEMENT_FACTOR;
	player->velocity.y += y_direction * MOVEMENT_FACTOR;
}

void	remove_velocity(t_player *player, double x_direction, double y_direction)
{
	player->velocity.x -= x_direction * MOVEMENT_FACTOR;
	player->velocity.y -= y_direction * MOVEMENT_FACTOR;
}

void	reset_velocity(t_player *player)
{
	player->velocity.x = 0;
	player->velocity.y = 0;
}

void	rotate_player(t_player *player, int direction)
{
	double	player_x_dir;
	double	player_y_dir;
	double	cam_x_dir;
	double	cam_y_dir;

	reset_velocity(player);
	player_x_dir = player->direction.x;
	player_y_dir = player->direction.y;
	cam_x_dir = player->cam_plane.x;
	cam_y_dir = player->cam_plane.y;
	player->direction.x = (player_x_dir * COS_ROTATION - player_y_dir * (SIN_ROTATION * direction));
	player->direction.y = (player_x_dir * (SIN_ROTATION * direction) + player_y_dir * COS_ROTATION);
	player->cam_plane.x = (cam_x_dir * COS_ROTATION - cam_y_dir * (SIN_ROTATION * direction));
	player->cam_plane.y = (cam_x_dir * (SIN_ROTATION * direction) + cam_y_dir * COS_ROTATION);
	if (player->directions.forward == 1)
		add_velocity(player, player->direction.x, player->direction.y);
	if (player->directions.backward == 1)
		add_velocity(player, -player->direction.x, -player->direction.y);
	if (player->directions.left == 1)
		add_velocity(player, player->direction.y, -player->direction.x);
	if (player->directions.right == 1)
		add_velocity(player, -player->direction.y, player->direction.x);
}
