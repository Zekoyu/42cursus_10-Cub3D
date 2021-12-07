/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 18:05:45 by mframbou          #+#    #+#             */
/*   Updated: 2021/12/07 18:41:40 by mframbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <math.h>
# include <stdlib.h>
# include "mlx_keycodes.h"
#include "../minilibx/mlx.h"

#  define mapWidth 39
#  define mapHeight 24
#  define screenWidth 1600
#  define screenHeight 900
#  define MOVEMENT_FACTOR 0.075
#  define COS_ROTATION 0.99691733373
#  define SIN_ROTATION 0.07845909568
#  define BOUNDING_BOX_SIDE_SIZE 0.4
#  define SPRINT_SPEED_MULTIPLIER 2.5
//Ratio compared to the size of the screen (here it takes 1/10 of the screen)
//map will remain squared (take min(width/ratio), (height/ratio))
//Should not be set to les than 1
#  define MINIMAP_SIZE_RATIO 3.75
//Size of one tile represented on the minimap (here it's a square of 5x5)
#  define SIZE_OF_TILE_ON_MINIMAP 10
//player sprite size on minimap
#  define PLAYER_SIZE_ON_MINIMAP 12
#  define MINIMAP_BACKGROUND_COLOR 0xFF000000 //0x88FFFFFF
#  define MINIMAP_FOREGROUND_COLOR 0x00ede482 //0x00FF00BB
#  define MINIMAP_PLAYER_COLOR 0x00FF8855

/*
	Structs
*/
typedef struct s_vector
{
	double	x;
	double	y;
}	t_vector;

typedef struct s_vector3
{
	double	x;
	double	y;
	double	z;
}	t_vector3;

typedef struct s_point
{
	int	x;
	int	y;
}	t_point;

typedef struct	s_door
{
	float			closed;
	int				should_open;
	t_point			coords;
	struct s_door	*next;
}	t_door;


typedef struct s_img_data
{
	void	*img;
	char	*addr;
	int		bpp;
	int		line_length;
	int		endian;
}	t_img_data;

typedef struct s_directions
{
	int	forward;
	int	left;
	int	backward;
	int	right;
	int	rotate_l;
	int	rotate_r;
}	t_directions;

typedef struct s_player
{
	t_vector		pos;
	t_vector		direction;
	t_vector		cam_plane;
	t_vector3		velocity;
	t_directions	directions;
	double			speed;
}	t_player;

typedef struct s_ray
{
	t_vector	direction;
	t_vector	dda_distances;
	t_vector	total_distances;
	t_point		current_tile;
	t_point		direction_steps;
}	t_ray;

/*
	Side hit = 'x' OR 'y'
	Distance = distance to the wall
	Pos on wall = value between 0 and 1 representing the exact
		offset on the wall where the ray hit (0 = left 1 = right)
*/
typedef struct s_ray_hit
{
	char		side_hit;
	double		distance;
	double		wall_pos_hit;
	t_point		tile_hit;
	t_vector	direction;
}	t_ray_hit;

typedef struct s_texture
{
	t_img_data	image;
	int			width;
	int			height;
}	t_texture;

typedef struct s_draw_coords
{
	int	screen_x;
	int	line_height;
	int	draw_start;
	int	draw_end;
}	t_draw_coords;

typedef struct s_game
{
	t_img_data		main_img;
	t_img_data		minimap_img;
	t_player		player;
	int				**map;
	int				map_width;
	int				map_height;
	void			*mlx;
	void			*window;
	int				width;
	int				height;
	int				floor_color;
	int				ceil_color;
	t_texture		n_tex;
	t_texture		s_tex;
	t_texture		e_tex;
	t_texture		w_tex;
	t_texture		test1;
	t_texture		test2;
}	t_game;

// Vectors
double		degrees_to_radians(double angle);
double		get_x_direction(double angle);
double		get_y_direction(double angle);


// Key handling
int			key_press_handler(int keycode, t_game *game);
int			key_release_handler(int keycode, t_game *game);

// Mouse handling
int			mouse_hook(int keycode, t_game *game);
int			get_mouse_velocity(void *window);

// MLX Utils
void		mlx_put_pixel_img(t_img_data *img, int x, int y, int color);

// FT Utils
int			min(int a, int b);
double		power_two(double num);

// Minimap
void		add_minimap(t_img_data *minimap_img, int map[mapHeight][mapWidth], t_player player);

// Triangles (for minimap)
t_point		get_triangle_front_point(t_vector normalized_player_dir);
t_point		get_triangle_side_point(t_vector normalized_player_dir);
int			is_point_in_triangle(t_point pt, t_point v1, t_point v2, t_point v3);

// ↑↑↓↓←→←→AB
int			is_uuddlrlrab_complete(int code[10]);
int			check_uuddlrlrab(int keycode);


// Point utils
int			get_max_x(t_point t1, t_point t2, t_point t3);
int			get_max_y(t_point t1, t_point t2, t_point t3);
int			get_min_x(t_point t1, t_point t2, t_point t3);
int			get_min_y(t_point t1, t_point t2, t_point t3);

// Movements
void		add_velocity(t_player *player, double x_direction, double y_direction);
void		remove_velocity(t_player *player, double x_direction, double y_direction);
void		reset_velocity(t_player *player);
void		rotate_player(t_player *player, int direction);
void		add_player_movements(t_player *player);

// ↑↑↓↓←→←→AB
void		teleport_player(t_player *player);

// Rendering
void		drawline_from_distance(int x, t_ray_hit ray_hit, t_game *game);

// DDA Algorithm
t_vector	get_dda_distances(t_vector direction);
t_ray_hit	get_ray_hit(t_vector direction, int map[mapHeight][mapWidth], t_vector player_pos);

// Coordinates manipulation
t_point		get_pos_current_tile(t_vector player_pos);

// Hitbox
int			has_intersection_with_wall(t_vector player_pos, int map[mapHeight][mapWidth]);

// Frame counter
void	add_curent_frame(void);
int		get_current_frame(void);

// Doors
void	add_door(int x, int y);
t_door	*get_door(t_point tile);
int		is_door(t_point tile);
void	open_close_doors_if_needed(void);

#endif