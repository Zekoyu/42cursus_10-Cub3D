/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_handling_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/04 23:53:25 by mframbou          #+#    #+#             */
/*   Updated: 2021/12/13 09:19:31 by mframbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	mouse_click_handler(int keycode, int x, int y, t_game *game)
{
	if (check_uuddlrlrab(keycode))
		game->do_the_spin = 1;
	return (69420);
}

/*
	If mouse is almost outside the app in X
		(outside of the bounds of 10-90% of the screen height)
	Put it back to the center

	If the mouse is "almost" outside the app in Y
		(outside of the bounds of 25-75% of the screen height)

	we reset it as well so that it does not go over applications
		or something like that
	(only reset it on Y axis)

	The offset is between -Width/2 (= left), 0 = middle, +Width/2 = right
*/
static int	get_x_mouse_offset(void *window, int win_height, int win_width)
{
	int		initial_pos;
	int		current_x;
	int		current_y;
	int		offset;

	initial_pos = win_width / 2;
	mlx_mouse_get_pos(window, &current_x, &current_y);
	offset = current_x - initial_pos;
	if (current_y < win_height / 4 || current_y > win_height / 4)
		mlx_mouse_move(window, offset + (win_width / 2), win_height / 2);
	if (offset >= (win_width - win_width / 10) / 2 \
	|| offset <= -(win_width - win_width / 10) / 2)
		mlx_mouse_move(window, win_width / 2, win_height / 2);
	return (offset);
}

/*
	Tries to recreate a "smooth" velocity, not perfect but does the job

	Velocity is done by comparing last frame position with current frame position
	If the movement is too high (> 150 || < -150) do not smooth it
*/
int	get_mouse_velocity(t_game *game)
{
	static int	prev_pos = 0;
	static int	smooth_mouse_velo;
	int			current_pos;
	int			velo;

	if (game->paused)
	{
		smooth_mouse_velo = 0;
		prev_pos = 0;
		return (0);
	}
	current_pos = get_x_mouse_offset(game->window, game->height, game->width);
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
