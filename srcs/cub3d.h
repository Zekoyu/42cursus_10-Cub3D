/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 18:05:45 by mframbou          #+#    #+#             */
/*   Updated: 2021/12/09 19:10:48 by mframbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <math.h>
# include <stdlib.h>
# include "mlx_keycodes.h"
# include "../minilibx/mlx.h"

# define MOVEMENT_FACTOR 0.075
# define COS_ROTATION 0.99691733373
# define SIN_ROTATION 0.07845909568
/*
	Cos of rotation angle and sin of rotation angle (directly in radians)
	Current values for 

	2 degrees	= 0,0349066000	rad (cos = 0.99939082649, sin = 0.03489951165)
	3 degrees	= 0,0523599000	rad (cos = 0.99862953358, sin = 0.05233597865)
	3.5 degrees	= 0.0610865238	rad (cos = 0.99813479842, sin = 0.06104853951)
	4.5 degrees = 0.0785398163	rad (cos = 0.99691733373, sin = 0.07845909568)
	5 degrees	= 0,0872665000	rad (cos = 0.99619469483, sin = 0.08715578000)
*/
# define BOUNDING_BOX_SIDE_SIZE 0.4
# define SPRINT_SPEED_MULTIPLIER 2.5
//Ratio compared to the size of the screen (here it takes 1/10 of the screen)
//map will remain squared (take min(width/ratio), (height/ratio))
//Should not be set to les than 1
# define MINIMAP_SIZE_RATIO 3.75
//Size of one tile represented on the minimap (here it's a square of 5x5)
# define SIZE_OF_TILE_ON_MINIMAP 10
//player sprite size on minimap
# define PLAYER_SIZE_ON_MINIMAP 12
# define MINIMAP_BACKGROUND_COLOR 0xFF000000 //0x88FFFFFF
# define MINIMAP_FOREGROUND_COLOR 0x00ede482 //0x00FF00BB
# define MINIMAP_PLAYER_COLOR 0x00FF8855

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

typedef struct s_door
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

typedef struct s_map
{
	int	**map;
	int	total_width;
	int	total_height;
	int	width;
	int	height;
}	t_map;

typedef struct s_minimap
{
	t_img_data	img;
	int			size_px;
}	t_minimap;
typedef struct s_game
{
	int				dqwdqwdqwd;
	t_img_data		main_img;
	t_minimap		minimap;
	t_img_data		pause_screen;
	t_player		player;
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
	t_texture		bg1;
	t_texture		bg2;
	t_texture		door;
	int				paused;
	int				should_exit;
	t_map			map;
}	t_game;

// Vectors
double		degrees_to_radians(double angle);
double		get_x_direction(double angle);
double		get_y_direction(double angle);
void		normalize_vector(t_vector *vector);

// Key handling
int			key_press_handler(int keycode, t_game *game);
int			key_release_handler(int keycode, t_game *game);

// Mouse handling
int			mouse_hook(int keycode, t_game *game);
int			get_mouse_velocity(t_game *game);

// MLX Utils
void		mlx_put_pixel_img(t_img_data *img, int x, int y, int color);
int			mlx_get_pixel_img(t_img_data *img, int x, int y);
int			is_point_in_img_bounds(t_point px, int width, int height);

// FT Utils
int			min(int a, int b);
double		power_two(double num);

// Minimap
void		add_minimap(t_minimap *minimap, t_map map, t_player player);

// Triangles (for minimap)
t_point		get_triangle_front_point(t_vector normalized_player_dir);
t_point		get_triangle_side_point(t_vector normalized_player_dir);
int			is_point_in_triangle(t_point pt, t_point v1, \
								t_point v2, t_point v3);

// ↑↑↓↓←→←→AB
int			is_uuddlrlrab_complete(int code[10]);
int			check_uuddlrlrab(int keycode);
void		draw_special_texture(t_draw_coords draw_coords, t_ray_hit ray_hit, \
								t_game *game);

// Point utils
int			get_max_x(t_point t1, t_point t2, t_point t3);
int			get_max_y(t_point t1, t_point t2, t_point t3);
int			get_min_x(t_point t1, t_point t2, t_point t3);
int			get_min_y(t_point t1, t_point t2, t_point t3);

// Movements
void		add_velocity(t_player *player, double x_direction, \
											double y_direction);
void		remove_velocity(t_player *player, double x_direction, \
												double y_direction);
void		reset_velocity(t_player *player);
void		rotate_player(t_player *player, int direction);
void		add_player_movements(t_player *player);

// Rendering
void		drawline_from_distance(int x, t_ray_hit ray_hit, t_game *game);
void		draw_texture(t_draw_coords draw_coords, t_texture tx, \
						t_ray_hit hit, t_game *game);
void		do_render(t_game *game);

// DDA Algorithm
t_vector	get_dda_distances(t_vector direction);
t_ray_hit	get_ray_hit(t_vector direction, t_map map, \
						t_vector player_pos);
t_ray_hit	do_the_dda_algorithm(t_ray ray, t_map map, \
						t_vector player_pos);
// Coordinates manipulation
t_point		get_pos_current_tile(t_vector player_pos);
t_point		get_pos_current_tile_floor(t_vector player_pos);

// Hitbox
int			has_intersection_with_wall(t_vector player_pos, t_map map);

// Frame counter
void		add_curent_frame(void);
int			get_current_frame(void);

// Doors
void		add_door(int x, int y);
t_door		*get_door(t_point tile);
int			is_door(t_point tile);
void		open_close_doors_if_needed(t_vector player_pos);
void		reset_doors_opening(void);

// Map checking
int			is_enclosed_algo(int *map, t_point msize, t_point *stack, \
							t_point curr);

// Initialization
int			init_mlx_images_and_textures(t_game *game, char **textures_files);
int			init_textures_addresses(t_game *game);
int			init_minimap(t_game *game);
int			init_main_mlx_window_img(t_game *game, int width, int height, \
							char *win_name);
int			load_textures(t_game *game, char **textures_files, \
							int mahmoud, int zaraa);

// Freeation
void		free_main_win(t_game *game);
void		free_main_img(t_game *game);
void		free_minimap(t_game *game);
void		free_textures(t_game *game);

#endif