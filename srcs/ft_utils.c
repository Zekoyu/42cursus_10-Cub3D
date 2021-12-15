/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 23:12:26 by mframbou          #+#    #+#             */
/*   Updated: 2021/12/14 12:59:57 by mframbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

double	power_two(double num)
{
	return (num * num);
}

int	min(int a, int b)
{
	if (a < b)
		return (a);
	return (b);
}

/*
	For instance, number: 5.05, precision 0.1 > true, precision 0.01 > false
*/
int	is_almost_integer(double number, double precision)
{
	double	floored_num;
	double	floor_limit;
	double	ceil_limit;

	floored_num = floor(number);
	floor_limit = floored_num - precision;
	ceil_limit = floored_num + precision;
	return (number >= floor_limit && number <= ceil_limit);
}
