/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 18:05:21 by mframbou          #+#    #+#             */
/*   Updated: 24-02-2022 15:59 by                                             */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

#ifndef DO_BONUSES

/* 
"gngngn fait un lerp" #orondarnaque
inline double	ft_lerp(double min, double max, double val)
{
	return (min + val * (max - min));
}
*/

/*	     X
	+-------->
	|
  Y |
	|
	V
*/

/* Moving left / right (rotate direction vector by 90 to get velocity vector)
	Rotate a vector:
	x2 = x * cos(90) - y * sin(90)
	y2 = x * sin(90) + y * cos(90)

	x2 = x * cos(pi/2) - y * sin(pi/2)
	y2 = x * sin(pi/2) - y * cos(pi/2)

	x2 = x * 0 - y * 1
	y2 = x * 1 - y * 0

	x2 = 0 - y * 1
	y2 = x * 1 - 0

	(90 degrees)
	x2 = -y
	y2 = x

	(-90 degrees)
	x2 = y
	y2 = -x
*/

/* Rotating left / right

	x2 = x * cos(0.1) - y * sin (0.1)
	y2 = x * sin(0.1) + y * cos(0.1)

	x2 = x * 0.1 - y * 0.01
	y2 = x * 0.01 + y * 0.1
*/

/*
	Direction 1 = right, -1 = left
	0.1 radians ~= 5.7 degrees
	0.995 = cos(0.1)
	0.0998 = sin(0.1)
*/

/*
	If there is an intersection with a wall after going forward (or any side)
	Revert the movement (pos -= velocity since before we had pos += velocity)
	But don't reset the rotation, because we canstill look around
	if we hit a wall

	Since we do this before the render, the player will not "glitch out" in
		and out the wall
*/

static void	rotate_player_if_needed(t_game *game)
{
	if (game->player.directions.rotate_l == 1)
		rotate_player(&game->player, -1);
	if (game->player.directions.rotate_r == 1)
		rotate_player(&game->player, 1);
}

int	loop_hook(t_game *game)
{
	rotate_player_if_needed(game);
	game->player.pos.x += game->player.velocity.x * game->player.speed;
	game->player.pos.y += game->player.velocity.y * game->player.speed;
	if (game->player.pos.x <= 0. || game->player.pos.x >= game->map.width - 1. \
	|| game->player.pos.y <= 0. || game->player.pos.y >= game->map.height - 1.)
		game->should_exit = 1;
	if (game->should_exit)
		exit(1);
	do_render(game);
	return (1);
}

void	init_game(t_game *game, int width, int height)
{
	game->width = width;
	game->height = height;
	game->ceil_color = 0xFF000000;
	game->floor_color = 0xFF000000;
	game->player.pos.y = 1.5;
	game->player.pos.x = 1.5;
	game->player.direction.x = 1.0;
	game->player.direction.y = 0.0;
	game->player.cam_plane.x = 0.0;
	game->player.cam_plane.y = 1.0;
	game->player.velocity.x = 0.0;
	game->player.velocity.y = 0.0;
	game->player.velocity.z = 0.0;
	game->player.speed = 1.0;
	game->paused = 0;
	game->should_exit = 0;
	game->do_the_spin = 0;
	game->n_tex.image.img = NULL;
	game->e_tex.image.img = NULL;
	game->s_tex.image.img = NULL;
	game->w_tex.image.img = NULL;
}

int	close_win(t_game *game)
{
	game->should_exit = 1;
	return (69);
}

int	main(int argc, char **argv)
{
	t_game	game;

	if (argc < 2)
	{
		write(1, "Error\nNot enough arguments provided.\n", 38);
		return (EXIT_FAILURE);
	}
	init_game(&game, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (init_main_mlx_window_img(&game, SCREEN_WIDTH, SCREEN_HEIGHT, "Cub3D"))
		return (EXIT_FAILURE);
	if (parse_cub_file(argv[1], &game) == -1)
		return (EXIT_FAILURE);
	if (init_mlx_images_and_textures(&game))
		return (EXIT_FAILURE);
	do_render(&game);
	mlx_do_key_autorepeatoff(game.mlx);
	mlx_hook(game.window, 2, 0, &key_press_handler, &game);
	mlx_hook(game.window, 3, 0, &key_release_handler, &game);
	mlx_hook(game.window, 17, 0, &close_win, &game);
	mlx_loop_hook(game.mlx, &loop_hook, &game);
	mlx_loop(game.mlx);
}
#endif
