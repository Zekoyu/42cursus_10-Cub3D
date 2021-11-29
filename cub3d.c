/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 18:05:21 by mframbou          #+#    #+#             */
/*   Updated: 2021/11/29 16:38:03 by mframbou         ###   ########.fr       */
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
/*
int worldMap[mapWidth][mapHeight]=
{
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
  {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
  {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
  {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
  {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
  {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
  {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
  {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};
*/
/*	     X
	+-------->
	|
  Y |
	|
	V
*/

void	mlx_put_pixel_img(t_img_data *img, int x, int y, int color)
{
	char	*dst;

	dst = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
	*((unsigned int *) dst) = color;
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

	int drawStart = -line_height / 2 + screenHeight / 2;
    if(drawStart < 0)
		drawStart = 0;
    int drawEnd = line_height / 2 + screenHeight / 2;
	if(drawEnd >= screenHeight)
		drawEnd = screenHeight - 1;
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
			color = 0x00FFFF00;
			break; //yellow
	}
	if (side == 'y')
		color = color | 0x80000000;
	int y = 0;
	while (y < drawStart)
		mlx_pixel_put(game.mlx , game.window, x, y++, 0x00231570);
	while (y <= drawEnd)
		mlx_pixel_put(game.mlx , game.window, x, y++, color);
	while (y < screenHeight)
		mlx_pixel_put(game.mlx , game.window, x, y++, 0x00ede482);	
		
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

int main()
{
	game.mlx = mlx_init();
	game.window = mlx_new_window(game.mlx, screenWidth, screenHeight, "Cub3D");
	game.main_img.img = mlx_new_image(game.mlx, screenWidth, screenHeight);
	game.main_img.addr = mlx_get_data_addr(game.main_img.img, &game.main_img.bits_per_pixel, &game.main_img.line_length, &game.main_img.endian);
	
	
	game.player.pos.y = 6.5;
	game.player.pos.x = 1.5;
	game.player.direction.x = 0.8;
	game.player.direction.y = 0.2;
	game.player.cam_plane.x = 0.2;
	game.player.cam_plane.y = 0.8;


	t_vector	ray_dir;
	t_vector 	direction;

	double		dda_x_distance;
	double		dda_y_distance;
	
	double		camera_pos_on_plane;

	direction.x = game.player.direction.x;
	direction.y = game.player.direction.y;

	for (int x = 0; x < screenWidth; x++)
	{
		// Left of the sreen will have camera pos of -1, mid of 0, and right of 1
		// This corresponds to the camera X position on the camera plane
		// the camera "slides" along it to get all ray directions
		camera_pos_on_plane = (2.0 * x) / (double) screenWidth - 1;

		ray_dir.x = direction.x + (game.player.cam_plane.x * camera_pos_on_plane);
		ray_dir.y = direction.y + (game.player.cam_plane.y * camera_pos_on_plane);
		
		
		// If we are only traveling on Y axis, then we can tell that to go +1 on X
		// The distance will be almost infinite, this avoids division by 0
		
		//dda_x_distance = get_x_for_1y_step(ray_dir);
		//dda_y_distance = get_y_for_1x_step(ray_dir);
		
		if (ray_dir.x == 0)
			dda_x_distance = 1E30;
		else
			dda_x_distance = fabs(1.0 / ray_dir.x);
		
		if(ray_dir.y == 0)
			dda_y_distance = 1E30;
		else
			dda_y_distance = fabs(1.0 / ray_dir.y);
			
		//printf("Ray %d direction is (%f, %f), dda_x: %f, dda_y:%f\n", x, ray_dir.x, ray_dir.y, dda_x_distance, dda_y_distance);
		
		int curr_tile_x = (int) game.player.pos.x;
		int curr_tile_y = (int) game.player.pos.y;
		
		double	dist_x;
		double	dist_y;

		int	step_x;
		int	step_y;

		// Get the coordinate within the tile (between 0-1), then multiply it 
		// with our "normalized" dda distance
		// Depending on the side where we go, we should either use the cell coordinate as
		// it (for instance 0.2) or 1-coord (1-0.2 = 0.8) to get the distance
		t_vector	player_pos_in_tile;

		player_pos_in_tile.x = (game.player.pos.x - curr_tile_x);
		player_pos_in_tile.y = (game.player.pos.y - curr_tile_y);
		if (ray_dir.x < 0)
		{
			step_x = -1;
			dist_x = player_pos_in_tile.x * dda_x_distance;
		}
		else
		{
			step_x = 1;
			dist_x = (1.0 - player_pos_in_tile.x) * dda_x_distance;
		}

		if (ray_dir.y < 0)
		{
			step_y = -1;
			dist_y = player_pos_in_tile.y * dda_y_distance;
		}
		else
		{
			step_y = 1;
			dist_y = (1.0 - player_pos_in_tile.y) * dda_y_distance;
		}
		//printf("Ray %d initial dist X=%f, Y=%f\n", x, dist_x, dist_y);
		//printf("DDA Distance x:%f, y:%f\n",dda_x_distance, dda_y_distance);
		// Actual DDA
		int		hit = 0;
		char	side_hit = '\0';
		/*
			Always take the nearest hit, once hit is made, remove the distance traveled
			from the other distance (if we travel on horizontal, remove vertical distance)
			Once we hit a wall, reset the distance to the original value and continue
		*/
		double	total_dist = 0;
		while (!hit)
		{
			//printf("Current tile X:%d, Y:%d  - dist X:%f Y:%f\n", curr_tile_x, curr_tile_y, dist_x, dist_y);
			if (dist_x < dist_y)
			{
				/*dist_y -= dist_x;
				dist_x = dda_x_distance;
				total_dist += dist_x;
				*/
				dist_x += dda_x_distance;
				curr_tile_x += step_x;
				side_hit = 'x';
			}
			else
			{
				/*
				dist_x -= dist_y;
				dist_y = dda_y_distance;
				total_dist += dist_y;
				*/
				dist_y += dda_y_distance;
				curr_tile_y += step_y;
				side_hit = 'y';
			}
			
			if (worldMap[curr_tile_y][curr_tile_x] != 0)
				hit = 1;
		}
		/*if (side_hit == 'y')
		{
			printf("Ray %d hit a Y aligned wall on (%d, %d)\n", x, curr_tile_x, curr_tile_y);
		}
		else if (side_hit == 'x')
		{
			printf("Ray %d hit a X aligned wall on (%d, %d)\n", x, curr_tile_x, curr_tile_y);
		}
		else
		{
			printf("Ray %d did not hit any wall\n", x);
		}*/
		
		double	wall_dist;
		
		/*
			Remove one dda since we have one more than needed
			Like if we started on tile 1,1, then go to tile 1,2
			We are actually at the "end" of 1,2 and the wall is at the "start"
		*/
		
		/*double	perp_dist;
		double current_angle;
		double	dir_len = sqrt(pow_two(ray_dir.x) + pow_two(ray_dir.y));
		double	cam_distance = sqrt(pow_two(camera_pos_on_plane * game.player.cam_plane.x) + pow_two(camera_pos_on_plane * game.player.cam_plane.y));
		double	perp_to_cam_dist = sqrt(pow_two(dir_len) + pow_two(cam_distance));
		*/

		
		//printf("Dist X:%f Y:%f, dda X:%f Y:%f\n", dist_x, dist_y, dda_x_distance, dda_y_distance);
		if (side_hit == 'x')
		{
			wall_dist = dist_x /*- dda_x_distance*/;
			//current_angle = atan(ray_dir.y / ray_dir.x);
			//perp_dist =  perp_to_cam_dist / (dir_len / wall_dist);
		}
		else
		{
			wall_dist = dist_y /*- dda_y_distance*/;
			//perp_dist =  perp_to_cam_dist / (dir_len / wall_dist);
			//current_angle = atan(ray_dir.x / ray_dir.y);	
		}
		//perp_dist = sqrt(pow_two(wall_dist) + pow_two(dir_size));

		//perp_dist = wall_dist * (sin(M_PI_4) sin(current_angle);
		//wall_dist = sqrt(pow_two(wall_dist), - pow_two())
		
		//printf("Ray %d is going %f on y for %f length, total distance: %f\n", x, cam_distance, dir_len, wall_dist);
		//printf("Wall dist=%f, direction length=%f, walldist/dirlen=%f\n", wall_dist, dir_len, dir_len / wall_dist);
		//printf("Ray %d, perp_dist = %f, wall dist = %f\n", x, perp_dist, wall_dist);
		//printf("Cam distance: %f (ray dir: (%f, %f)) wall_dist: %f, dda:x=%f, y=%f, Corrected dist =%f\n", cam_distance, ray_dir.x, ray_dir.y, wall_dist, dda_x_distance, dda_y_distance, sqrt(pow_two(wall_dist) - pow_two(cam_distance)));
		//printf("Way %d hit (%d, %d) at distance %f\n", x, curr_tile_x, curr_tile_y, wall_dist);
		//wall_dist *= cos(current_angle);

		drawline_from_distance(x, wall_dist, worldMap[curr_tile_y][curr_tile_x], side_hit);
		//printf("Ray %d (%f, %f), Wall distance without correction: %f, with correction: %f (angle degree=%f radians=%f)\n", x, ray_dir.x, ray_dir.y, wall_dist, wall_dist*cos(current_angle), current_angle, cos(current_angle));
		//printf("Ray %d hit (%d ,%d) with an angle of %f and distance of : %f\n", x, curr_tile_x, curr_tile_y, current_angle, wall_dist);
	}
	mlx_loop(game.mlx);
	
	//mlx_loop(game.mlx);
}
