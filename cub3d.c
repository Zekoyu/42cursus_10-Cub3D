/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 18:05:21 by mframbou          #+#    #+#             */
/*   Updated: 2021/11/30 19:10:52 by mframbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minilibx/mlx.h"
#include "cub3d.h"
#include "mlx_keycodes.h"
#include <stdlib.h>

#define mapWidth 24
#define mapHeight 24
#define screenWidth 1280
#define screenHeight 720
#define MOVEMENT_FACTOR 0.075
#define COS_ROTATION 0.99862953358
#define SIN_ROTATION 0.05233597865
#define BOUNDING_BOX_SIDE_SIZE 0.4

// Cos of rotation angle and sin of rotation angle (directly in radians)
// Current values for 
/*
	2 degrees	= 0,0349066 	rad		(cos = 0.99939082649, sin = 0.03489951165)
	3 degrees	= 0,0523599		rad		(cos = 0.99862953358, sin = 0.05233597865)
	3.5 degrees	= 0.0610865238 	rad		(cos = 0.99813479842, sin = 0.06104853951)
	5 degrees	= 0,0872665 	rad		(cos = 0.99619469483, sin = 0.08715578)
*/
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

t_point	get_pos_current_tile(t_vector player_pos)
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
	ray.current_tile = get_pos_current_tile(player_pos);
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

void	add_velocity(t_player *player, double x_direction, double y_direction)
{
	player->velocity.x += x_direction * MOVEMENT_FACTOR;
	player->velocity.y += y_direction * MOVEMENT_FACTOR;
}

void	remove_velocity(t_player *player, double x_direction, double y_direction)
{
	player->velocity.x -= x_direction * MOVEMENT_FACTOR;
	player->velocity.y -= y_direction * MOVEMENT_FACTOR;
}

void	reset_velocity(t_player *player)
{
	player->velocity.x = 0;
	player->velocity.y = 0;
}
/*
	Direction 1 = right, -1 = left
	0.1 radians ~= 5.7 degrees
	0.995 = cos(0.1)
	0.0998 = sin(0.1)
*/
void	rotate_player(t_player *player, int direction)
{
	double	player_x_dir;
	double	player_y_dir;
	double	cam_x_dir;
	double	cam_y_dir;

	reset_velocity(player);
	player_x_dir = player->direction.x;
	player_y_dir = player->direction.y;
	cam_x_dir = player->cam_plane.x;
	cam_y_dir = player->cam_plane.y;
	player->direction.x = (player_x_dir * COS_ROTATION - player_y_dir * (SIN_ROTATION * direction));
	player->direction.y = (player_x_dir * (SIN_ROTATION * direction) + player_y_dir * COS_ROTATION);
	player->cam_plane.x = (cam_x_dir * COS_ROTATION - cam_y_dir * (SIN_ROTATION * direction));
	player->cam_plane.y = (cam_x_dir * (SIN_ROTATION * direction) + cam_y_dir * COS_ROTATION);
	if (player->directions.forward == 1)
		add_velocity(player, player->direction.x, player->direction.y);
	if (player->directions.backward == 1)
		add_velocity(player, -player->direction.x, -player->direction.y);
	if (player->directions.left == 1)
		add_velocity(player, player->direction.y, -player->direction.x);
	if (player->directions.right == 1)
		add_velocity(player, -player->direction.y, player->direction.x);
}



/*
	Rotate_player function automatically readjust velocity
*/
int	key_press_event(int keycode, t_game *game)
{
	t_player	*player;

	if (keycode == KEY_ESC)
		exit(0);
	player = &game->player;
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
	else if (keycode == KEY_ARROW_LEFT)
	{
		player->directions.rotate_l = 1;
		rotate_player(player, -1);
	}
	else if (keycode == KEY_ARROW_RIGHT)
	{
		player->directions.rotate_r = 1;
		rotate_player(player, 1);
	}
}





/* OLD
	if (player->directions.forward == 1)
	{
		player->velocity.x += player->direction.x * MOVEMENT_FACTOR;
		player->velocity.y += player->direction.y * MOVEMENT_FACTOR;
	}
	else if (player->directions.backward == 1)
	{
		player->velocity.x += -(player->direction.x * MOVEMENT_FACTOR);
		player->velocity.y += -(player->direction.y * MOVEMENT_FACTOR);
	}
	else if (player->directions.left == 1)
	{
		player->velocity.x += (player->direction.y * MOVEMENT_FACTOR);
		player->velocity.y += -(player->direction.x * MOVEMENT_FACTOR);
	}
	else if (player->directions.right == 1)
	{
		player->velocity.x += -(player->direction.y * MOVEMENT_FACTOR);
		player->velocity.y += (player->direction.x * MOVEMENT_FACTOR);
	}
	else if (player->directions.rotate_l == 1)
	{
		rotate_player(player, -1);
	}
	else if (player->directions.rotate_r == 1)
	{
		rotate_player(player, 1);
	}
*/
/*
	Before rotating, remove the actual velocity, rotate it and put it back
*/

void	add_player_movements(t_player *player)
{
	if (player->directions.forward == 1)
		add_velocity(player, player->direction.x, player->direction.y);
	else if (player->directions.backward == 1)
		add_velocity(player, -player->direction.x, -player->direction.y);
	else if (player->directions.left == 1)
		add_velocity(player, player->direction.y, -player->direction.x);
	else if (player->directions.right == 1)
		add_velocity(player, -player->direction.y, player->direction.x);
	else if (player->directions.rotate_l == 1)
	{
		reset_velocity(player);
		rotate_player(player, -1);
	}
	else if (player->directions.rotate_r == 1)
	{
		reset_velocity(player);
		rotate_player(player, 1);
	}
}

int	point_intersects_wall(t_vector point, int map[24][24])
{
	t_point	current_tile;

	current_tile = get_pos_current_tile(point);
	if (map[current_tile.y][current_tile.x] != 0)
		return (1);
	return (0);
}

int	has_intersection_with_wall(t_vector player_pos, int map[24][24])
{
	t_vector	bounding_box_bot_left;
	t_vector	bouding_box_bot_right;
	t_vector	bounding_box_top_left;
	t_vector	bouding_box_top_right;
	t_point		current_tile;

	bounding_box_bot_left.x = player_pos.x - BOUNDING_BOX_SIDE_SIZE / 2;
	bounding_box_bot_left.y = player_pos.y - BOUNDING_BOX_SIDE_SIZE / 2;
	bouding_box_bot_right.x = player_pos.x + BOUNDING_BOX_SIDE_SIZE / 2;
	bouding_box_bot_right.y = player_pos.y - BOUNDING_BOX_SIDE_SIZE / 2;
	bouding_box_top_right.x = player_pos.x + BOUNDING_BOX_SIDE_SIZE / 2;
	bouding_box_top_right.y = player_pos.y + BOUNDING_BOX_SIDE_SIZE / 2;
	bounding_box_top_left.x = player_pos.x - BOUNDING_BOX_SIDE_SIZE / 2;
	bounding_box_top_left.y = player_pos.y + BOUNDING_BOX_SIDE_SIZE / 2;
	
	if (point_intersects_wall(bounding_box_top_left, map) \
		|| point_intersects_wall(bouding_box_top_right, map) \
		|| point_intersects_wall(bounding_box_bot_left, map) \
		|| point_intersects_wall(bouding_box_bot_right, map))
	{
		return (1);
	}
	return (0);
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
	
	/*
		struct timeval start;
		gettimeofday(&start, NULL);
		print_elapsed("\nstart: ", start);
	*/
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

/*
	If there is an intersection with a wall after going forward (or any side)
	Revert the movement (pos -= velocity since before we had pos += velocity)
	But don't reset the rotation, because we canstill look around
	if we hit a wall
*/

// mlx_mouse_get_pos(window, int *x, int *y)
int	get_x_mouse_offset(void *window)
{
	int	initial_pos;
	int	current_pos;
	int	trucinutile;
	int	offset;

	initial_pos = screenWidth / 2;
	mlx_mouse_get_pos(window, &current_pos, &trucinutile);
	offset = current_pos - initial_pos;
	// If mouse is more than 9/10 or less than 1/10 of the window put it back at center
	if (offset >= (screenWidth - screenWidth / 10) / 2 || offset <= -(screenWidth - screenWidth / 10) / 2)
		mlx_mouse_move(window, screenWidth / 2, screenHeight / 2);
	else
		return (offset);
}

// If too much offset, we probably went from side to middle
/*
int	get_mouse_velocity(void *window)
{
	static int	prev_pos = 0;
	static int smooth_mouse_velo;
	int	current_pos;
	int	tmp;
	int	velo;

	current_pos = get_x_mouse_offset(window);
	tmp = prev_pos;
	prev_pos = current_pos;
	velo = current_pos - tmp;
	//printf("velo: %d, smooth velo: %d\n", velo, smooth_mouse_velo);
	if (smooth_mouse_velo - velo <= 100)
	{
		if (velo < 0)
			smooth_mouse_velo = velo;
		else if (velo > 0)
			smooth_mouse_velo = velo;
	}
	else
	{
		smooth_mouse_velo -= smooth_mouse_velo % 3;
		if (smooth_mouse_velo < 0)
			smooth_mouse_velo += 3;
		else if (smooth_mouse_velo > 0)
			smooth_mouse_velo -= 3;
		return (velo);
	}
	smooth_mouse_velo -= smooth_mouse_velo % 3;
	if (smooth_mouse_velo < 0)
		smooth_mouse_velo += 3;
	else if (smooth_mouse_velo > 0)
		smooth_mouse_velo -= 3;
	return (smooth_mouse_velo);
}
*/

int	get_mouse_velocity(void *window)
{
	static int	prev_pos = 0;
	static int smooth_mouse_velo;
	int	current_pos;
	int	tmp;
	int	velo;

	current_pos = get_x_mouse_offset(window);
	tmp = prev_pos;
	prev_pos = current_pos;
	velo = current_pos - tmp;
	if ((velo < 0 && velo > -50) || (velo > 0 && velo < 50))
		smooth_mouse_velo = velo;
	smooth_mouse_velo -= smooth_mouse_velo % 5;
	if (smooth_mouse_velo < 0)
		smooth_mouse_velo -= 5;
	else if (smooth_mouse_velo > 0)
		smooth_mouse_velo += 5;
	return (smooth_mouse_velo);
}

int loop_hook(t_game *game)
{
	
	//mlx_mouse_move(game->window, screenWidth / 2, screenHeight / 2);
	//printf("current pos (%f, %f), velocity (%f %f)\n", game->player.pos.x, game->player.pos.y, game->player.velocity.x, game->player.velocity.y);
	int mouse_velo = get_mouse_velocity(game->window);
	//printf("mouse velocity: %d\n", mouse_velo);
	
	if (mouse_velo <= -5)
	{
		game->player.directions.rotate_l = 1;
		game->player.directions.rotate_r = 0;
	}
	else if (mouse_velo >= 5)
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
	game->player.pos.x += game->player.velocity.x;
	game->player.pos.y += game->player.velocity.y;
	if (has_intersection_with_wall(game->player.pos, worldMap))
	{
		game->player.pos.y -= game->player.velocity.y;
		game->player.pos.x -= game->player.velocity.x;
		/*if (game->player.directions.rotate_r == 1)
			rotate_player(&game->player, -1);
		if (game->player.directions.rotate_l == 1)
			rotate_player(&game->player, 1);
			*/
	//	printf("reset pos (%f, %f), velocity (%f %f)\n", game->player.pos.x, game->player.pos.y, game->player.velocity.x, game->player.velocity.y);
	}
	
	do_render(game);
	return (1);
}

int	key_release_event(int keycode, t_game *game)
{
	t_player	*player;

	player = &game->player;
	if (keycode == KEY_W)
	{
		game->player.directions.forward = 0;
		remove_velocity(player, player->direction.x, player->direction.y);
	}
	else if (keycode == KEY_S)
	{
		game->player.directions.backward = 0;
		remove_velocity(player, -player->direction.x, -player->direction.y);
	}
	else if (keycode == KEY_A)
	{
		game->player.directions.left = 0;
		remove_velocity(player, player->direction.y, -player->direction.x);
	}
	else if (keycode == KEY_D)
	{
		game->player.directions.right = 0;
		remove_velocity(player, -player->direction.y, player->direction.x);
	}
	else if (keycode == KEY_ARROW_LEFT)
	{
		game->player.directions.rotate_l = 0;
	}
	else if (keycode == KEY_ARROW_RIGHT)
	{
		game->player.directions.rotate_r = 0;
	}
}

int main()
{
	game.mlx = mlx_init();
	game.window = mlx_new_window(game.mlx, screenWidth, screenHeight, "Cub3D");
	game.main_img.img = mlx_new_image(game.mlx, screenWidth, screenHeight);
	game.main_img.addr = mlx_get_data_addr(game.main_img.img, &game.main_img.bits_per_pixel, &game.main_img.line_length, &game.main_img.endian);
	
	mlx_mouse_move(game.window, screenWidth / 2, screenHeight / 2);
	mlx_mouse_hide();
	
	game.player.pos.y = 12.5;
	game.player.pos.x = 1.5;
	game.player.direction.x = 1.0;
	game.player.direction.y = 0.0;
	game.player.cam_plane.x = 0.0;
	game.player.cam_plane.y = 1.0;

	do_render(&game);

	
	mlx_do_key_autorepeatoff(game.mlx);
	
	mlx_hook(game.window, 2, 0, &key_press_event, &game);
	mlx_hook(game.window, 3, 0, &key_release_event, &game);
	mlx_loop_hook(game.mlx, &loop_hook, &game);
	mlx_loop(game.mlx);
	
	//mlx_loop(game.mlx);
}
