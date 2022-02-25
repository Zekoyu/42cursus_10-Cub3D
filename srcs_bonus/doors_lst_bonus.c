/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doors_lst.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 13:29:07 by mframbou          #+#    #+#             */
/*   Updated: 25-02-2022 12:36 by                                             */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

t_door	**get_door_lst(void)
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
