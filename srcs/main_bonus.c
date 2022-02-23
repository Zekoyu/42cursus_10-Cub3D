/* ************************************************************************** */
/*                                                                            */
/*                                  .-.                       .               */
/*                                 / -'                      /                */
/*                  .  .-. .-.   -/--).--..-.  .  .-. .-.   /-.  .-._.)  (    */
/*   By:             )/   )   )  /  /    (  |   )/   )   ) /   )(   )(    )   */
/*                  '/   /   (`.'  /      `-'-''/   /   (.'`--'`-`-'  `--':   */
/*   Created: 23-02-2022  by  `-'                        `-'                  */
/*   Updated: 23-02-2022 17:57 by                                             */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

#define DO_BONUSES

#ifdef DO_BONUSES
typedef struct s_sprite
{
	void	*mlx;
	void	*animation[51];
	int		index;
}	t_sprite;

t_sprite animation;
animation.index = 0;

int	loop_hook(t_game *game)
{
	int mouse_velo = get_mouse_velocity(game);
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
	{
		rotate_player(&game->player, -1);
	}
	if (game->player.directions.rotate_r == 1)
		rotate_player(&game->player, 1);
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
	{
		printf("Exit ? TODO");
		exit(1);
	}
	do_render(game);
	if (!game->paused)
	{
		if ((fabs(game->player.velocity.x) > 0.01 || fabs(game->player.velocity.y) > 0.01) && game->player.speed > 1.0 && animation.animation[animation_index] != NULL)
		{
			mlx_put_image_to_window(game->mlx, game->window, animation.animation[animation_index], 0, 0);
			if (get_current_frame() % 3 == 0)
				animation_index++;
			if (animation_index > 50)
				animation_index = 0;
		}
		open_close_doors_if_needed(game->player.pos);
		add_curent_frame();
	}
	else
	{
		mlx_put_image_to_window(game->mlx, game->window, game->pause_screen.img, 0, 0);
	}
	#endif
	return (1);
}


void	init_game(t_game *game, int width, int height)
{
	game->width = width;
	game->height = height;

	game->ceil_color = 0x00231570;
	game->floor_color = 0x00ede482;

	game->player.pos.y = 12.5;
	game->player.pos.x = 1.5;

	game->player.direction.x = 1.0;
	game->player.direction.y = 0.0;

	game->player.cam_plane.x = 0.0;
	game->player.cam_plane.y = 1.0;

	game->player.speed = 1.0;
	game->paused = 0;
	game->should_exit = 0;
	game->do_the_spin = 0;
}

#include <string.h>
#include <stdio.h>

void	*load_animated_sprites(void *arg)
{
	t_sprite	*sprite;

	sprite = (t_sprite *) arg;
	void	**animation = sprite->animation;

	for (int i = 0; i <= 50; i++)
	{
		char filename[60] = "./speedlines_xpm_900-1600px/speedline_";
		char num[4];
		sprintf(num, "%d", i);
		strcat(filename, num);
		strcat(filename, ".xpm");
		int nul;
		animation[i] = mlx_xpm_file_to_image(sprite->mlx, filename, &nul, &nul);
		if (!animation[i])
			return ((void *) -1);
	}
	return (0);
}

int	close_win(t_game *game)
{
	game->should_exit = 1;
	return (69);
}

#include <pthread.h>

static int	init_bg(void *mlx, t_texture *tex, char *filename)
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

	if (init_bg(game->mlx, &(game->bg1), "./1.xpm") == -1)
		return (-1);
	if (init_bg(game->mlx, &(game->bg2), "./2.xpm") == -1)
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
	int	pouet;
	int	x;
	int	y;

	game->pause_screen.img = mlx_xpm_file_to_image(game->mlx, \
							"./pause_menu_1600-900px.xpm", &pouet, &pouet);
	if (!game->pause_screen.img)
		return (-1);
	game->pause_screen.addr = mlx_get_data_addr(game->pause_screen.img, \
				&game->pause_screen.bpp, &game->pause_screen.line_length, \
				&game->pause_screen.endian);
	if (!game->pause_screen.addr)
	{
		mlx_destroy_image(game->mlx, game->pause_screen.img);
		return (-1);
	}
	x = -1;
	while (++x < 1600)
	{
		y = -1;
		while (++y < 900)
			if (((unsigned int *) game->pause_screen.addr)[y * 1600 + x] == \
																	0xFF000000)
				((unsigned int *) game->pause_screen.addr)[y * 1600 + x] = \
																	0x88888888;
	}
	return (0);
}

int main(int argc, char **argv)
{
	t_game game;

	init_game(&game, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (init_main_mlx_window_img(&game, SCREEN_WIDTH, SCREEN_HEIGHT, "Cub3D"))
		return (EXIT_FAILURE);
	/*
	#ifdef DO_BONUSES
	add_door(6, 5);
	#endif
	*/

	if (init_beaux_gosses(&game) == -1)
		return (EXIT_FAILURE);

	pthread_t init_img_thread;

	for (int i = 0; i <= 50; i++)
	{
		animation.animation[i] = NULL;
	}
	animation.mlx = game.mlx;

	pthread_create(&init_img_thread, NULL, &load_animated_sprites, &animation);
	
	if (init_pause_screen(&game) == -1)
	{
		return (EXIT_FAILURE);
	}

	#ifdef DO_BONUSES
	game.door.image.img = mlx_xpm_file_to_image(game.mlx, "./door_tex.xpm", &x_size, &y_size);
	game.door.height = y_size;
	game.door.width = x_size;

	game.door.image.addr = mlx_get_data_addr(game.door.image.img, \
				&game.door.image.bpp, &game.door.image.line_length, \
				&game.door.image.endian);


	
	mlx_mouse_move(game.window, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	mlx_mouse_hide();
	mlx_mouse_hook(game.window, &mouse_click_handler, &game);
	#endif
	
	if (argc < 2)
	{
		printf("not enough args\n");
		return (1);
	}
	game.ceil_color = 0xFF000000;
	game.floor_color = 0xFF000000;
	game.n_tex.image.img = NULL;
	game.e_tex.image.img = NULL;
	game.s_tex.image.img = NULL;
	game.w_tex.image.img = NULL;
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
	pthread_join(init_img_thread, NULL);
}

#endif