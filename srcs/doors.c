/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doors.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 17:52:27 by mframbou          #+#    #+#             */
/*   Updated: 2021/12/08 16:07:30 by mframbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static t_door	**get_door_lst(void)
{
	static t_door	*lst = NULL;

	return (&lst);
}

t_door	*get_door(t_point tile)
{
	t_door	*lst;

	lst = *get_door_lst();
	while (lst)
	{
		if (lst->coords.x == tile.x && lst->coords.y == tile.y)
			return (lst);
		lst = lst->next;
	}
	return (NULL);
}

/*
	Door pos 1.0 = closed
	0.0 = opened
	Door opens from right to left
*/
void	add_door(int x, int y)
{
	t_door	*door;
	t_door	*lst;

	door = malloc(sizeof(t_door));
	door->closed = 1.0;
	door->coords.x = x;
	door->coords.y = y;
	door->should_open = 0;
	door->next = NULL;
	lst = *get_door_lst();
	if (lst)
	{
		while (lst->next)
			lst = lst->next;
		lst->next = door;
	}
	else
		*get_door_lst() = door;
}

int	is_door(t_point tile)
{
	t_door	*lst;

	lst = *get_door_lst();
	while (lst)
	{
		if (lst->coords.x == tile.x && lst->coords.y == tile.y)
			return (1);
		lst = lst->next;
	}
	return (0);
}

double	get_door_dist_to_player(t_door *door, t_vector pos)
{
	double	x_diff;
	double	y_diff;

	x_diff = (double) door->coords.x - pos.x;
	y_diff = (double) door->coords.y - pos.y;
	return (sqrt(power_two(x_diff) + power_two(y_diff)));
}

void	open_close_doors_if_needed(t_vector player_pos)
{
	t_door	*curr_door;

	curr_door = *get_door_lst();
	while (curr_door)
	{
		if (get_door_dist_to_player(curr_door, player_pos) <= 3.0)
			curr_door->should_open = 1;
		else
			curr_door->should_open = -1;
		if (curr_door->should_open == 1)
		{
			curr_door->closed -= 0.025;
			if (curr_door->closed < 0.0)
				curr_door->closed = 0.0;
		}
		else if (curr_door->should_open == -1)
		{
			curr_door->closed += 0.025;
			if (curr_door->closed > 1.0)
				curr_door->closed = 1.0;
		}
		curr_door->should_open = 0;
		curr_door = curr_door->next;
	}
}
