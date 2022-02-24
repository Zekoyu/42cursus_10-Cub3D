/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_map_algorithm.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 16:42:32 by mframbou          #+#    #+#             */
/*   Updated: 24-02-2022 19:27 by                                             */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	add_stack_dir_x(t_point *stack, int x_dir, int stack_height, \
							t_point current)
{
	stack[stack_height].x = current.x + x_dir;
	stack[stack_height].y = current.y;
}

static void	add_stack_dir_y(t_point *stack, int y_dir, int stack_height, \
							t_point current)
{
	stack[stack_height].x = current.x;
	stack[stack_height].y = current.y + y_dir;
}

/*
	Msize = map size
*/
int	is_enclosed_algo(int *map, t_point msize, t_point *stack, \
							t_point curr)
{
	int	stack_height;

	stack_height = 0;
	stack[stack_height].x = curr.x;
	stack[stack_height++].y = curr.y;
	stack_height = 1;
	while (stack_height > 0)
	{
		curr.x = stack[stack_height - 1].x;
		curr.y = stack[stack_height - 1].y;
		stack_height--;
		if (curr.x >= 0 && curr.x < msize.x && curr.y >= 0 && curr.y < msize.y)
		{
			if (map[curr.y * msize.x + curr.x] != 0)
				continue ;
			map[curr.y * msize.x + curr.x] = -1;
			add_stack_dir_x(stack, 1, stack_height++, curr);
			add_stack_dir_x(stack, -1, stack_height++, curr);
			add_stack_dir_y(stack, 1, stack_height++, curr);
			add_stack_dir_y(stack, -1, stack_height++, curr);
		}
		else
			return (0);
	}
	return (1);
}
