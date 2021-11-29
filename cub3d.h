/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 18:05:45 by mframbou          #+#    #+#             */
/*   Updated: 2021/11/29 16:23:52 by mframbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <math.h>
# include <stdlib.h>

typedef struct s_vector
{
	double	x;
	double	y;
}	t_vector;

typedef struct s_point
{
	int	x;
	int	y;
}	t_point;

typedef struct s_img_data
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_img_data;

typedef struct s_player
{
	t_vector	pos;
	t_vector	direction;
	t_vector	cam_plane;
}	t_player;

/*
	Side hit = 'x' OR 'y'
	Distance = distance to the wall
	Pos on wall = value between 0 and 1 representing the exact
		offset on the wall where the ray hit (0 = left 1 = right)
*/
typedef struct s_ray_hit
{
	char	side_hit;
	double	distance;
}	t_ray_hit;

typedef struct s_game
{
	void		*mlx;
	void		*window;
	t_img_data	main_img;
	t_player	player;
}	t_game;

// Vectors
double			degrees_to_radians(double angle);
double			get_x_direction(double angle);
double			get_y_direction(double angle);

#endif