/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 18:05:21 by mframbou          #+#    #+#             */
/*   Updated: 2021/12/09 18:07:04 by mframbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	do_render(t_game *game);

void *animation[51];
int animation_index = 0;

#define mapWidth 39
#define mapHeight 24
#define screenWidth 1600
#define screenHeight 900

int worldMap[mapHeight][mapWidth]=
{
	{0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,1,1,1,1,1,1,4,4,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{4,0,0,0,0,0,4,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,4,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,4,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,4,4,4,4,4,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};
t_game		game;


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
int teleport_test = 0;
void	teleport_player(t_player *player)
{
	teleport_test = 1;
}


void	do_render(t_game *game)
{
	t_vector	ray_dir;
	t_vector 	direction;
	double		dda_x_distance;
	double		dda_y_distance;
	double		camera_pos_on_plane;
	
	direction.x = game->player.direction.x;
	direction.y = game->player.direction.y;
	static double current_angle = 0.007;
	static double multiplier = 1.07;
	if (teleport_test != 0)
	{
		t_vector	player_dir;
		t_vector	cam_dir;
		double		current_cos;
		double		current_sin;

		current_cos = cos(current_angle);
		current_sin = sin(current_angle);

		player_dir.x = game->player.direction.x;
		player_dir.y = game->player.direction.y;
		cam_dir.x = game->player.cam_plane.x;
		cam_dir.y = game->player.cam_plane.y;
		game->player.direction.x = (player_dir.x * current_cos - player_dir.y * current_sin );
		game->player.direction.y = (player_dir.x * current_sin + player_dir.y * current_cos);
		game->player.cam_plane.x = (cam_dir.x * current_cos - cam_dir.y * current_sin);
		game->player.cam_plane.y = (cam_dir.x * current_sin + cam_dir.y * current_cos);

		reset_velocity(&game->player);
		current_angle *= multiplier;
		if (teleport_test == 1)
		{
			if (current_angle > M_PI * 2)
			{
				teleport_test = -1;
				multiplier = 0.93;
			}
		}
		else if (teleport_test == -1)
		{
			game->player.pos.x = 27.5;
			game->player.pos.y = 10.5;
			if (current_angle <= 0.007)
			{
				multiplier = 1.07;
				teleport_test = 0;
				current_angle = 0.007;
				{
					reset_velocity(&game->player);
					add_player_movements(&game->player);
				}
			}
		}
	}
	t_ray_hit ray_hit;
	for (int x = 0; x < screenWidth; x++)
	{
		camera_pos_on_plane = (2.0 * x) / (double) screenWidth - 1;
		ray_dir.x = direction.x + (game->player.cam_plane.x * camera_pos_on_plane);
		ray_dir.y = direction.y + (game->player.cam_plane.y * camera_pos_on_plane);
		ray_hit = get_ray_hit(ray_dir, game->map, game->player.pos);
		drawline_from_distance(x, ray_hit, game);
	}
	add_minimap(&game->minimap, game->map, game->player);
	mlx_clear_window(game->mlx, game->window);
	mlx_put_image_to_window(game->mlx, game->window, game->main_img.img, 0, 0);
	mlx_put_image_to_window(game->mlx, game->window, game->minimap.img.img, 0, 0);
}

/*
	If there is an intersection with a wall after going forward (or any side)
	Revert the movement (pos -= velocity since before we had pos += velocity)
	But don't reset the rotation, because we canstill look around
	if we hit a wall

	Since we do this before the render, the player will not "glitch out" in and out the wall
*/
int loop_hook(t_game *game)
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
		if ((fabs(game->player.velocity.x) > 0.01 || fabs(game->player.velocity.y) > 0.01) && game->player.speed > 1.0 && animation[animation_index] != NULL)
		{
			mlx_put_image_to_window(game->mlx, game->window, animation[animation_index], 0, 0);
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
	game->player.cam_plane.y = 0.78;

	game->player.speed = 1.0;
	game->paused = 0;
	game->should_exit = 0;
}

#include <string.h>
#include <stdio.h>


void	*load_animated_sprites(void *arg)
{
	void	**animation = (void **) arg;

	for (int i = 0; i <= 50; i++)
	{
		char filename[60] = "./speedlines_xpm_900-1600px/speedline_";
		char num[4];
		sprintf(num, "%d", i);
		strcat(filename, num);
		strcat(filename, ".xpm");
		int nul;
		animation[i] = mlx_xpm_file_to_image(game.mlx, filename, &nul, &nul);
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
int main()
{
	char *textures_files[] =
	{
		"./N_tex.xpm",
		"./S_tex.xpm",
		"./E_tex.xpm",
		"./W_tex.xpm"
	};
	init_game(&game, screenWidth, screenHeight);
	if (init_main_mlx_window_img(&game, screenWidth, screenHeight, "Cub3D"))
		return (-1);
	if (init_mlx_images_and_textures(&game, (char **)textures_files))
		return (-1);

	add_door(6, 5);
	add_door(6, 6);
	
	int	x_size;
	int	y_size;

	pthread_t init_img_thread;

	for (int i = 0; i <= 50; i++)
	{
		animation[i] = NULL;
	}

	pthread_create(&init_img_thread, NULL, &load_animated_sprites, animation);
	
	game.bg1.image.img = mlx_xpm_file_to_image(game.mlx, "./1.xpm", &x_size, &y_size);
	game.bg1.height = y_size;
	game.bg1.width = x_size;

	game.bg1.image.addr = mlx_get_data_addr(game.bg1.image.img, \
				&game.bg1.image.bpp, &game.bg1.image.line_length, \
				&game.bg1.image.endian);

	game.bg2.image.img = mlx_xpm_file_to_image(game.mlx, "./2.xpm", &x_size, &y_size);
	game.bg2.height = y_size;
	game.bg2.width = x_size;

	game.bg2.image.addr = mlx_get_data_addr(game.bg2.image.img, \
				&game.bg2.image.bpp, &game.bg2.image.line_length, \
				&game.bg2.image.endian);

	game.pause_screen.img = mlx_xpm_file_to_image(game.mlx, "./pause_menu_1600-900px.xpm", &x_size, &y_size);

	game.pause_screen.addr = mlx_get_data_addr(game.pause_screen.img, \
				&game.pause_screen.bpp, &game.pause_screen.line_length, \
				&game.pause_screen.endian);
	for (int x = 0; x < 1600; x++)
	{
		for (int y = 0; y < 900; y++)
		{
			if (((unsigned int *) game.pause_screen.addr)[y * 1600 + x] == 0xFF000000)
			{
				((unsigned int *) game.pause_screen.addr)[y * 1600 + x] = 0x88888888;
			}
		}
	}

	game.map.total_width = mapWidth;
	game.map.total_height = mapHeight;

	game.map.height = 24;
	game.map.width = 24;
	
	game.map.map = (int **) malloc(sizeof(int *) * mapHeight);
	for (int y = 0; y < mapHeight; y++)
	{
		game.map.map[y] = (int *) malloc(sizeof(int) * mapWidth);
		for (int x = 0; x < mapWidth; x++)
		{
			game.map.map[y][x] = worldMap[y][x];
		}
	}

	game.door.image.img = mlx_xpm_file_to_image(game.mlx, "./door_tex.xpm", &x_size, &y_size);
	game.door.height = y_size;
	game.door.width = x_size;

	game.door.image.addr = mlx_get_data_addr(game.door.image.img, \
				&game.door.image.bpp, &game.door.image.line_length, \
				&game.door.image.endian);


	mlx_mouse_move(game.window, screenWidth / 2, screenHeight / 2);
	mlx_mouse_hide();

	do_render(&game);
	mlx_do_key_autorepeatoff(game.mlx);
	
	mlx_hook(game.window, 2, 0, &key_press_handler, &game);
	mlx_hook(game.window, 3, 0, &key_release_handler, &game);
	mlx_hook(game.window, 17, 0, &close_win, &game);

	mlx_mouse_hook(game.window, &mouse_hook, &game);

	mlx_loop_hook(game.mlx, &loop_hook, &game);
	mlx_loop(game.mlx);

	pthread_join(init_img_thread, NULL);
}
