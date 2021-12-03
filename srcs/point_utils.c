/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   point_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 23:48:10 by mframbou          #+#    #+#             */
/*   Updated: 2021/12/03 16:01:15 by mframbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	get_min_y(t_point t1, t_point t2, t_point t3)
{
	int	y1;
	int	y2;
	int	y3;

	y1 = t1.y;
	y2 = t2.y;
	y3 = t3.y;
	if (y1 <= y2 && y1 <= y3)
		return (y1);
	else if (y2 <= y1 && y2 <= y3)
		return (y2);
	else
		return (y3);
}

int	get_max_y(t_point t1, t_point t2, t_point t3)
{
	int	y1;
	int	y2;
	int	y3;

	y1 = t1.y;
	y2 = t2.y;
	y3 = t3.y;
	if (y1 >= y2 && y1 >= y3)
		return (y1);
	else if (y2 >= y1 && y2 >= y3)
		return (y2);
	else
		return (y3);
}

int	get_min_x(t_point t1, t_point t2, t_point t3)
{
	int	x1;
	int	x2;
	int	x3;

	x1 = t1.x;
	x2 = t2.x;
	x3 = t3.x;
	if (x1 <= x2 && x1 <= x3)
		return (x1);
	else if (x2 <= x1 && x2 <= x3)
		return (x2);
	else
		return (x3);
}

int	get_max_x(t_point t1, t_point t2, t_point t3)
{
	int	x1;
	int	x2;
	int	x3;

	x1 = t1.x;
	x2 = t2.x;
	x3 = t3.x;
	if (x1 >= x2 && x1 >= x3)
		return (x1);
	else if (x2 >= x1 && x2 >= x3)
		return (x2);
	else
		return (x3);
}
