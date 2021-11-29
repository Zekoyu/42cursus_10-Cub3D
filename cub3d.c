/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 18:05:21 by mframbou          #+#    #+#             */
/*   Updated: 2021/11/29 19:05:17 by mframbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include "cub3d.h"
#include "mlx_keycodes.h"
#include <stdlib.h>

#define mapWidth 24
#define mapHeight 24
#define screenWidth 1280
#define screenHeight 720
#define FOV_DEGREES 90

int worldMap[mapWidth][mapHeight]=
{
  {1,1,4,2,4,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {3,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {4,0,0,0,0,0,2,4,4,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {4,0,0,0,0,0,4,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,4,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,4,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};
/*	     X
	+-------->
	|
  Y |
	|
	V
*/

#include <sys/time.h>
#include <stdio.h>

void print_elapsed(char str[], struct timeval start)
{
	struct timeval now;

	gettimeofday(&now, NULL);
	int diff_s = now.tv_sec - start.tv_sec;
	int diff_u = now.tv_usec - start.tv_usec;
	printf("%s %d elapsed\n", str, (diff_s * 1000 * 1000 + diff_u));
}

void	mlx_put_pixel_img(t_img_data *img, int x, int y, int color)
{
	char	*dst;

	dst = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
	*((unsigned int *)dst) = color;
}

inline double	ft_lerp(double min, double max, double val)
{
	return (min + val * (max - min));
}

#include <stdio.h>

double	pow_two(double num)
{
	return (num * num);
}

t_game		game;

void	drawline_from_distance(int x, double distance, int wall_type, char side)
{
	int	line_height = round(((double)screenHeight / distance));
	unsigned int	color = 0x00000FFF;

	/*struct timeval start;
	gettimeofday(&start, NULL);
	print_elapsed("start: ", start);*/

	int drawStart = -line_height / 2 + screenHeight / 2;
    if(drawStart < 0)
		drawStart = 0;
    int drawEnd = line_height / 2 + screenHeight / 2;
	if(drawEnd >= screenHeight)
		drawEnd = screenHeight - 1;
	
	//print_elapsed("calculs: ", start);
	switch(wall_type)
	{
		case 1:
			color = 0x00FF0000;
			break; //red
        case 2:
			color = 0x0000FF00;
			break; //green
        case 3:
			color = 0x000000FF;
			break; //blue
        case 4:
			color = 0x00FFFFFF;
			break; //white
        default:
			color = wall_type;
			break; //yellow
	}
	if (side == 'y')
		color = color | 0x88000000;
	//print_elapsed("colors: ", start);
	int y = 0;

	//unsigned int	*img_addr;

	//img_addr = (unsigned int *) game.main_img.addr;

	while (y < drawStart)
	{
		//img_addr[x + y * screenWidth] = 0x00231570;
		//y++;
		mlx_put_pixel_img(&game.main_img, x, y++, 0x00231570);
	}
	while (y <= drawEnd)
	{
		//img_addr[x + y * screenWidth] = color;
		//y++;
		mlx_put_pixel_img(&game.main_img, x, y++, color);
	}
	while (y < screenHeight)
	{
		//img_addr[x + y * screenWidth] = 0x00ede482;
		//y++;
		mlx_put_pixel_img(&game.main_img, x, y++, 0x00ede482);
	}
	//print_elapsed("put pixels: ", start);
	// Around 10us using mlx put pixel to put all pixels for a line (start 0 calculs 2 colors 4 put pixels 14)
}

/*
	Return the Y needed to go from X to X+1 based on the given direction
	If the vector is going straight top / bottom, then the needed Y to go to the next
	X is infinite (since it's going straight it will never encounter any horizontal lines)
	So return a very very high value
*/
double	get_y_for_1x_step(t_vector direction)
{
	if (direction.y != 0)
		return (sqrt(1 + pow_two(direction.x) / pow_two(direction.y)));
	else
		return (10E35);
}

/*
	Return the X needed to go fmor Y to Y+1 based on the given direction
	If the vector is going straight right / left, then the needed X to go to the next
	Y is infinite (since it's going straight it will never encounter any vertical lines)
*/
double	get_x_for_1y_step(t_vector direction)
{
	if (direction.x != 0)
		return (sqrt(1 + pow_two(direction.y) / pow_two(direction.x)));
	else
		return (10E35);
}

static double get_dda_x_distance(t_vector direction)
{
	if (direction.x == 0)
		return (1E30);
	else
		return (fabs(1.0 / direction.x));
}

static double get_dda_y_distance(t_vector direction)
{
	if (direction.y == 0)
		return (1E30);
	else
		return (fabs(1.0 / direction.y));
}

t_vector	get_dda_distances(t_vector direction)
{
	t_vector	res;

	res.x = get_dda_x_distance(direction);
	res.y = get_dda_y_distance(direction);
	return (res);
}

t_point	get_player_current_tile(t_vector player_pos)
{
	t_point	res;

	res.x = (int) player_pos.x;
	res.y = (int) player_pos.y;
	return (res);
}

t_point	get_direction_steps(t_vector direction)
{
	t_point	step;

	if (direction.x < 0)
		step.x = -1;
	else
		step.x = 1;
	if (direction.y < 0)
		step.y = -1;
	else
		step.y = 1;
	return (step);
}

// player position relative to the tile (between 0-1; 0-1)
// if ray is going left, then relative position = distance from left (default)
// if ray is going right, relative position = distance from right (1 - position)
t_vector	init_base_distances(t_vector direction, t_vector dda_distances, t_vector player_pos)
{
	t_vector	distances;
	t_vector	player_relative_pos;

	player_relative_pos.x = (player_pos.x - (double)((int) player_pos.x));
	player_relative_pos.y = (player_pos.y - (double)((int) player_pos.y));
	if (direction.x < 0)
		distances.x = player_relative_pos.x * dda_distances.x;
	else
		distances.x = (1.0 - player_relative_pos.x) * dda_distances.x;
	if (direction.y < 0)
		distances.y = player_relative_pos.y * dda_distances.y;
	else
		distances.y = (1.0 - player_relative_pos.y) * dda_distances.y;
	return (distances);
}

static t_ray_hit	get_ray_distance_side(t_ray ray, int map[24][24])
{
	t_ray_hit	ray_hit;

	while (1)
	{
		if (ray.total_distances.x < ray.total_distances.y)
		{
			ray.total_distances.x += ray.dda_distances.x;
			ray.current_tile.x += ray.direction_steps.x;
			ray_hit.side_hit = 'x';
		}
		else
		{
			ray.total_distances.y += ray.dda_distances.y;
			ray.current_tile.y += ray.direction_steps.y;
			ray_hit.side_hit = 'y';
		}
		if (map[ray.current_tile.y][ray.current_tile.x] != 0)
			break ;
	}
	if (ray_hit.side_hit == 'x')
		ray_hit.distance = ray.total_distances.x - ray.dda_distances.x;
	else
		ray_hit.distance = ray.total_distances.y - ray.dda_distances.y;
	ray_hit.tile_hit = ray.current_tile;
	return (ray_hit);
}



t_ray_hit	get_ray_hit(t_vector direction, int map[24][24], t_vector player_pos)
{
	t_ray		ray;
	t_ray_hit	ray_hit;

	//struct timeval start;

	//print_elapsed("start:", start);
	ray.direction = direction;
	ray.current_tile = get_player_current_tile(player_pos);
	//print_elapsed("get tile:", start);
	ray.dda_distances = get_dda_distances(ray.direction);
	//print_elapsed("get dda:", start);
	ray.direction_steps = get_direction_steps(ray.direction);
	//print_elapsed("get steps:", start);
	ray.total_distances = init_base_distances(ray.direction, \
											ray.dda_distances, player_pos);
	//print_elapsed("get base distances:", start);
	ray_hit = get_ray_distance_side(ray, map);
	//print_elapsed("dda:", start);
	return (ray_hit);
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
	
	/*struct timeval start;
	gettimeofday(&start, NULL);
	print_elapsed("\nstart: ", start);*/
	for (int x = 0; x < screenWidth; x++)
	{
		camera_pos_on_plane = (2.0 * x) / (double) screenWidth - 1;
		ray_dir.x = direction.x + (game->player.cam_plane.x * camera_pos_on_plane);
		ray_dir.y = direction.y + (game->player.cam_plane.y * camera_pos_on_plane);
		t_ray_hit ray_hit = get_ray_hit(ray_dir, worldMap, game->player.pos);
		if (x == 0)
		{
			//printf("ray hit distance: %f\n", ray_hit.distance);
		}
		//print_elapsed("\ncalcul: ", start);
		drawline_from_distance(x, ray_hit.distance, worldMap[ray_hit.tile_hit.y][ray_hit.tile_hit.x], ray_hit.side_hit);
		//print_elapsed("draw: ", start);
	}
	mlx_clear_window(game->mlx, game->window);
	mlx_put_image_to_window(game->mlx, game->window, game->main_img.img, 0,0);	
	//print_elapsed("put window: ", start);
}

int	key_hook(int keycode, t_game *game)
{
	game->player.pos.x += 0.5;
	do_render(game);
	return (1);
}

int loop_hook(t_game *game)
{
	game->player.pos.x += 0.05;
	game->player.pos.y += 0.02;
	do_render(game);
	return (1);
}

int main()
{
	game.mlx = mlx_init();
	game.window = mlx_new_window(game.mlx, screenWidth, screenHeight, "Cub3D");
	game.main_img.img = mlx_new_image(game.mlx, screenWidth, screenHeight);
	game.main_img.addr = mlx_get_data_addr(game.main_img.img, &game.main_img.bits_per_pixel, &game.main_img.line_length, &game.main_img.endian);
	
	game.player.pos.y = 12.5;
	game.player.pos.x = 1.5;
	game.player.direction.x = 1.0;
	game.player.direction.y = 0.0;
	game.player.cam_plane.x = 0.0;
	game.player.cam_plane.y = 1.0;

	do_render(&game);
	
	//mlx_key_hook(game.window, &key_hook, &game);
	mlx_loop_hook(game.mlx, &loop_hook, &game);
	mlx_loop(game.mlx);
	
	//mlx_loop(game.mlx);
}
