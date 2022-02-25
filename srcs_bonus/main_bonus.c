/* ************************************************************************** */
/*                                                                            */
/*                                  .-.                       .               */
/*                                 / -'                      /                */
/*                  .  .-. .-.   -/--).--..-.  .  .-. .-.   /-.  .-._.)  (    */
/*   By:             )/   )   )  /  /    (  |   )/   )   ) /   )(   )(    )   */
/*                  '/   /   (`.'  /      `-'-''/   /   (.'`--'`-`-'  `--':   */
/*   Created: 23-02-2022  by  `-'                        `-'                  */
/*   Updated: 25-02-2022 13:50 by                                             */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

#ifdef DO_BONUSES

# include <pthread.h>

int		init_all(t_game *game, pthread_t *anim_thread, char *cub_file);
void	init_game(t_game *game, int width, int height);

static void	rotate_player_if_needed(t_game *game)
{
	int	mouse_velo;

	mouse_velo = get_mouse_velocity(game);
	if (mouse_velo <= -2)
	{
		game->player.directions.rotate_l = 1;
		game->player.directions.rotate_r = 0;
	}
	else if (mouse_velo >= 2)
	{
		game->player.directions.rotate_l = 0;
		game->player.directions.rotate_r = 1;
	}
	else
	{
		game->player.directions.rotate_l = 0;
		game->player.directions.rotate_r = 0;
	}
	if (game->player.directions.rotate_l == 1)
		rotate_player(&game->player, -1);
	if (game->player.directions.rotate_r == 1)
		rotate_player(&game->player, 1);
}

static void	show_speedline_anim_if_needed(t_game *game)
{
	if ((fabs(game->player.velocity.x) > 0.01 \
		|| fabs(game->player.velocity.y) > 0.01) \
		&& game->player.speed > 1.0 \
		&& game->animation.animation[game->animation.index] != NULL)
	{
		mlx_put_image_to_window(game->mlx, game->window, \
						game->animation.animation[game->animation.index], 0, 0);
		if (get_current_frame() % 3 == 0)
			game->animation.index++;
		if (game->animation.index > 50)
			game->animation.index = 0;
	}
}

static int	loop_hook(t_game *game)
{
	rotate_player_if_needed(game);
	if (!game->paused)
	{
		game->player.pos.x += game->player.velocity.x * game->player.speed;
		game->player.pos.y += game->player.velocity.y * game->player.speed;
		if (has_intersection_with_wall(game->player.pos, game->map))
		{
			game->player.pos.y -= game->player.velocity.y * game->player.speed;
			game->player.pos.x -= game->player.velocity.x * game->player.speed;
		}
	}
	if (game->should_exit)
		exit(1);
	do_render(game);
	if (!game->paused)
	{
		show_speedline_anim_if_needed(game);
		open_close_doors_if_needed(game->player.pos);
		add_curent_frame();
	}
	else
		mlx_put_image_to_window(game->mlx, game->window, \
								game->pause_screen.img, 0, 0);
	return (1);
}

int	close_win(t_game *game)
{
	game->should_exit = 1;
	return (69);
}

int	main(int argc, char **argv)
{
	t_game		game;
	pthread_t	init_img_thread;

	if (argc < 2)
	{
		write(1, "Error\nNot enough arguments provided.\n", 38);
		return (EXIT_FAILURE);
	}
	init_game(&game, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (init_main_mlx_window_img(&game, SCREEN_WIDTH, SCREEN_HEIGHT, "Cub3D"))
		return (EXIT_FAILURE);
	if (init_all(&game, &init_img_thread, argv[1]) == -1)
		return (EXIT_FAILURE);
	mlx_mouse_move(game.window, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	mlx_mouse_hide();
	mlx_mouse_hook(game.window, &mouse_click_handler, &game);
	do_render(&game);
	mlx_do_key_autorepeatoff(game.mlx);
	mlx_hook(game.window, 2, 0, &key_press_handler, &game);
	mlx_hook(game.window, 3, 0, &key_release_handler, &game);
	mlx_hook(game.window, 17, 0, &close_win, &game);
	mlx_loop_hook(game.mlx, &loop_hook, &game);
	mlx_loop(game.mlx);
	pthread_join(init_img_thread, NULL);
}
#endif