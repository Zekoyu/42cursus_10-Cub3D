/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_press_handling.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 22:52:06 by mframbou          #+#    #+#             */
/*   Updated: 25-02-2022 12:36 by                                             */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

/* 
	Nice forest
    ^                ^
   /|\         ^    /|\            ^
      ^    ^  /|\         ^       /|\   ^
  ^  /|\  /|\      ^     /|\     ^	   /|\
 /|\              /|\           /|\
*/
/*
	- = Increase FOV (~= dezoom)
	+ = Decrease FOV (~= zoom)
*/
static void	key_press_handler_3(int keycode, t_player *player)
{
	if (keycode == KEY_SHFT)
		player->speed *= (double) SPRINT_SPEED_MULTIPLIER;
	else if (keycode == KEY_CTRL)
		player->speed /= (double) SPRINT_SPEED_MULTIPLIER * 2.0;
	if (keycode == KEY_ARROW_LEFT)
	{
		player->directions.rotate_l = 1;
		rotate_player(player, -1);
	}
	else if (keycode == KEY_ARROW_RIGHT)
	{
		player->directions.rotate_r = 1;
		rotate_player(player, 1);
	}
	else if (keycode == KEY_MINUS)
	{
		player->cam_plane.x *= 1.1;
		player->cam_plane.y *= 1.1;
	}
	else if (keycode == KEY_PLUS)
	{
		player->cam_plane.x *= 0.9;
		player->cam_plane.y *= 0.9;
	}
}

static void	key_press_handler_2(int keycode, t_player *player)
{
	if (keycode == KEY_W)
	{
		player->directions.forward = 1;
		add_velocity(player, player->direction.x, player->direction.y);
	}
	else if (keycode == KEY_S)
	{
		player->directions.backward = 1;
		add_velocity(player, -player->direction.x, -player->direction.y);
	}
	else if (keycode == KEY_A)
	{
		player->directions.left = 1;
		add_velocity(player, player->direction.y, -player->direction.x);
	}
	else if (keycode == KEY_D)
	{
		player->directions.right = 1;
		add_velocity(player, -player->direction.y, player->direction.x);
	}
	else
		key_press_handler_3(keycode, player);
}

int	key_press_handler(int keycode, t_game *game)
{
	t_player	*player;

	player = &game->player;
	if (check_uuddlrlrab(keycode))
		game->do_the_spin = 1;
	else if (keycode == KEY_P)
	{
		game->paused = !game->paused;
		if (game->paused)
		{
			mlx_mouse_move(game->window, game->width / 2, game->height / 2);
			mlx_mouse_show();
		}
		else
			mlx_mouse_hide();
	}
	else if (keycode == KEY_ESC)
	{
		game->should_exit = 1;
	}
	else
		key_press_handler_2(keycode, player);
	return (69);
}
