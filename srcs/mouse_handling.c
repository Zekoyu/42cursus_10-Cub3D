/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/04 23:53:25 by mframbou          #+#    #+#             */
/*   Updated: 2021/12/04 23:58:46 by mframbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	mouse_hook(int keycode, t_game *game)
{
	if (check_uuddlrlrab(keycode))
		teleport_player(game->player);
}

/*
	If mouse is almost outside the app in X (in the bounds of 10-90% of the screen height)
	Put it back to the center

	If the mouse is "almost" outside the app in Y (in the bounds of 25-75% of the screen height)
	we reset it as well so that it does not go over applications or something like that
	(only reset it on Y axis)

	The offset is between -Width/2 (= left), 0 = middle, +Width/2 = right
*/
static int	get_x_mouse_offset(void *window)
{
	int	initial_pos;
	int	current_x;
	int	current_y;
	int	offset;

	initial_pos = screenWidth / 2;
	mlx_mouse_get_pos(window, &current_x, &current_y);
	offset = current_x - initial_pos;
	if (current_y < screenHeight / 4 || current_y > screenHeight / 4)
		mlx_mouse_move(window, offset + (screenWidth / 2), screenHeight / 2);
	if (offset >= (screenWidth - screenWidth / 10) / 2 || offset <= -(screenWidth - screenWidth / 10) / 2)
		mlx_mouse_move(window, screenWidth / 2, screenHeight / 2);
	else
		return (offset);
}

/*
	Tries to recreate a "smooth" velocity, not perfect but does the job

	Velocity is done by comparing last frame position with current frame position
	If the movement is too high (> 150 || < -150) do not smooth it
*/
int	get_mouse_velocity(void *window)
{
	static int	prev_pos = 0;
	static int	smooth_mouse_velo;
	int			current_pos;
	int			velo;

	current_pos = get_x_mouse_offset(window);
	velo = current_pos - prev_pos;
	prev_pos = current_pos;
	if ((velo < 0 && velo > -150) || (velo > 0 && velo < 150))
		smooth_mouse_velo = velo;
	smooth_mouse_velo -= smooth_mouse_velo % 3;
	if (smooth_mouse_velo > 0)
		smooth_mouse_velo -= 3;
	else if (smooth_mouse_velo < 0)
		smooth_mouse_velo += 3;
	return (smooth_mouse_velo);
}

