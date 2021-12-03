/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_release_handling.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 22:59:52 by mframbou          #+#    #+#             */
/*   Updated: 2021/12/03 15:23:18 by mframbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/* Nice forest
    ^                ^
   /|\         ^    /|\            ^
      ^    ^  /|\         ^       /|\   ^
  ^  /|\  /|\      ^     /|\     ^	   /|\
 /|\              /|\           /|\
*/
static int	key_release_handler_2(int keycode, t_player *player)
{
	if (keycode == KEY_SHFT)
	{
		player->speed /= (double) SPEED_MULTIPLIER;
	}
	else if (keycode == KEY_CTRL)
	{
		player->speed *= (double) SPEED_MULTIPLIER;
	}
	else if (keycode == KEY_ARROW_LEFT)
	{
		player->directions.rotate_l = 0;
	}
	else if (keycode == KEY_ARROW_RIGHT)
	{
		player->directions.rotate_r = 0;
	}
}

int	key_release_handler(int keycode, t_game *game)
{
	t_player	*player;

	player = &game->player;
	if (keycode == KEY_W)
	{
		player->directions.forward = 0;
		remove_velocity(player, player->direction.x, player->direction.y);
	}
	else if (keycode == KEY_S)
	{
		player->directions.backward = 0;
		remove_velocity(player, -player->direction.x, -player->direction.y);
	}
	else if (keycode == KEY_A)
	{
		player->directions.left = 0;
		remove_velocity(player, player->direction.y, -player->direction.x);
	}
	else if (keycode == KEY_D)
	{
		player->directions.right = 0;
		remove_velocity(player, -player->direction.y, player->direction.x);
	}
	else
		key_release_handler_2(keycode, player);
}
