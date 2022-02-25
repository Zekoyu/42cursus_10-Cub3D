/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 12:25:02 by mframbou          #+#    #+#             */
/*   Updated: 25-02-2022 15:49 by                                             */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int			is_enclosed_algo(int *map, t_point msize, t_point *stack, \
							t_point curr);

/*
	Clones the map and puts all walls to 1 and empty / doors to 0
*/
static int	*clone_map_bzero(int **map, int width, int height)
{
	int	*map_clone;
	int	i;
	int	j;

	map_clone = (int *) malloc(sizeof(int *) * (width * height));
	i = 0;
	while (i < height)
	{
		j = 0;
		while (j < width)
		{
			if (map[i][j] == 0 || map[i][j] == -1)
				map_clone[i * width + j] = 0;
			else
				map_clone[i * width + j] = 1;
			j++;
		}
		i++;
	}
	return (map_clone);
}

/*
	Since we check the 4 adjacent, each point can be checked at more 4 times
	So init the stack to the total count of points in the map * 4
*/
static t_point	*init_stack(int map_width, int map_height)
{
	t_point	*stack;
	int		i;

	stack = malloc(sizeof(t_point) * (map_width * map_height * 4));
	if (stack)
	{
		i = 0;
		while (i < map_width * map_height * 4)
		{
			stack[i].x = 0;
			stack[i].y = 0;
			i++;
		}
	}
	return (stack);
}

/*
	Starts on player tile, check all 4 adjacent tiles
	If those are wall, don't continue, else check again the 4 adjacent

	If we manage to reach the outside of the map,
	it means that there is a wall missing
	
	else if we always stay in the map for all the tiles, it's fine

	Use a stack instead of recursion since it's more optimized
*/
int	is_enclosed(int **original_map, int width, int height, t_point player)
{
	t_point	dimensions;
	t_point	*stack;
	int		*map;
	int		is_enclosed;

	map = clone_map_bzero(original_map, width, height);
	stack = init_stack(width, height);
	dimensions.x = width;
	dimensions.y = height;
	is_enclosed = is_enclosed_algo(map, dimensions, stack, player);
	free(map);
	return (is_enclosed);
}
