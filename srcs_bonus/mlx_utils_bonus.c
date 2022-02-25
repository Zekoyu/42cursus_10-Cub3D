/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 23:07:39 by mframbou          #+#    #+#             */
/*   Updated: 25-02-2022 12:36 by                                             */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

void	mlx_put_pixel_img(t_img_data *img, int x, int y, int color)
{
	char	*dst;

	dst = img->addr + (y * img->line_length + x * (img->bpp / 8));
	*((unsigned int *)dst) = color;
}

int	mlx_get_pixel_img(t_img_data *img, int x, int y)
{
	char	*dst;

	dst = img->addr + (y * img->line_length + x * (img->bpp / 8));
	return (*((int *)dst));
}

/*
	Not used anymore
*/
void	mlx_put_line_to_img(t_img_data *img,
	t_point start, t_point end, int color)
{
	float		delta_x;
	float		delta_y;
	int			pixels_count;
	float		pixel_x;
	float		pixel_y;

	delta_x = end.x - start.x;
	delta_y = end.y - start.y;
	pixels_count = (int) sqrtl(delta_x * delta_x + delta_y * delta_y);
	delta_x = delta_x / (float) pixels_count;
	delta_y = delta_y / (float) pixels_count;
	pixel_x = start.x;
	pixel_y = start.y;
	while (pixels_count)
	{
		mlx_put_pixel_img(img, (int) lround(pixel_x),
			(int) lround(pixel_y), color);
		pixel_x += delta_x;
		pixel_y += delta_y;
		pixels_count--;
	}
}

unsigned int	rgb_to_hex(int r, int g, int b)
{
	return (r << 16 | g << 8 | b);
}
