/* ************************************************************************** */
/*                                                                            */
/*                                  .-.                       .               */
/*                                 / -'                      /                */
/*                  .  .-. .-.   -/--).--..-.  .  .-. .-.   /-.  .-._.)  (    */
/*   By:             )/   )   )  /  /    (  |   )/   )   ) /   )(   )(    )   */
/*                  '/   /   (`.'  /      `-'-''/   /   (.'`--'`-`-'  `--':   */
/*   Created: 23-02-2022  by  `-'                        `-'                  */
/*   Updated: 24-02-2022 16:19 by                                             */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

#ifdef DO_BONUSES

# define SPEEDLINES_PATH "./speedlines_xpm_900-1600px/speedline_"
# define DOOR_TEXTURE "./door_tex.xpm"

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

void	show_speedline_anim_if_needed(t_game *game)
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

int	loop_hook(t_game *game)
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

void	*load_animated_sprites(void *arg)
{
	t_sprite	*sprite;
	char		*num;
	static char	file[60] = SPEEDLINES_PATH;
	int			i;
	int			z;

	sprite = (t_sprite *) arg;
	i = -1;
	while (++i <= 50)
	{
		num = ft_itoa(i);
		ft_strlcat(file, num, 60);
		free(num);
		ft_strlcat(file, ".xpm", 60);
		sprite->animation[i] = mlx_xpm_file_to_image(sprite->mlx, file, &z, &z);
		file[ft_strlen(SPEEDLINES_PATH)] = '\0';
		if (!sprite->animation[i])
		{
			while (--i >= 0)
				mlx_destroy_image(sprite->mlx, sprite->animation[i]);
			return ((void *) -1);
		}
	}
	return (NULL);
}

int	close_win(t_game *game)
{
	game->should_exit = 1;
	return (69);
}

#include <pthread.h>

static int	init_tex(void *mlx, t_texture *tex, char *filename)
{
	int	x_size;
	int	y_size;

	tex->image.img = mlx_xpm_file_to_image(mlx, filename, \
												&x_size, &y_size);
	if (!tex->image.img)
		return (-1);
	tex->height = y_size;
	tex->width = x_size;
	tex->image.addr = mlx_get_data_addr(tex->image.img, \
				&tex->image.bpp, &tex->image.line_length, \
				&tex->image.endian);
	if (!tex->image.addr)
	{
		mlx_destroy_image(mlx, tex->image.img);
		return (-1);
	}
	return (0);
}

static int	init_beaux_gosses(t_game *game)
{
	int	x_size;
	int	y_size;

	if (init_tex(game->mlx, &(game->bg1), "./1.xpm") == -1)
		return (-1);
	if (init_tex(game->mlx, &(game->bg2), "./2.xpm") == -1)
	{
		mlx_destroy_image(game->mlx, game->bg1.image.img);
		return (-1);
	}
	return (0);
}

/*
	Initialize pause screen replacing transparent pixels with semi transparent
*/
static int	init_pause_screen(t_game *game)
{
	int		pouet;
	t_point	px;

	game->pause_screen.img = mlx_xpm_file_to_image(game->mlx, \
							"./pause_menu_1600-900px.xpm", &pouet, &pouet);
	if (!game->pause_screen.img)
		return (-1);
	game->pause_screen.addr = mlx_get_data_addr(game->pause_screen.img, \
				&game->pause_screen.bpp, &game->pause_screen.line_length, \
				&game->pause_screen.endian);
	if (!game->pause_screen.addr)
		return (-1);
	px.x = -1;
	while (++(px.x) < 1600)
	{
		px.y = -1;
		while (++(px.y) < 900)
			if (((int *) game->pause_screen.addr) \
						[px.y * 1600 + px.x] == 0xFF000000)
				((int *) game->pause_screen.addr) \
						[px.y * 1600 + px.x] = 0x88888888;
	}
	return (0);
}

void	init_animation(pthread_t *init_img_thread, t_sprite *animation, \
						void *mlx)
{
	int			i;

	i = 0;
	while (i <= 50)
		animation->animation[i++] = NULL;
	animation->mlx = mlx;
	pthread_create(init_img_thread, NULL, &load_animated_sprites, animation);
}

int	init_door_tex(t_game *game)
{
	if (init_tex(game->mlx, &(game->door), DOOR_TEXTURE) == -1)
		return (-1);
	return (0);
}

int main(int argc, char **argv)
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
	if (init_beaux_gosses(&game) == -1)
		return (EXIT_FAILURE);
	if (init_pause_screen(&game) == -1)
		return (EXIT_FAILURE);
	if (init_door_tex(&game) == -1)
		return (EXIT_FAILURE);
	init_animation(&init_img_thread, &(game.animation), game.mlx);

	if (parse_cub_file(argv[1], &game) == -1)
		return (EXIT_FAILURE);
	if (init_mlx_images_and_textures(&game))
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