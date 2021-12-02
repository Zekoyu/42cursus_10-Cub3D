/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 23:12:26 by mframbou          #+#    #+#             */
/*   Updated: 2021/12/02 23:29:53 by mframbou         ###   ########.fr       */
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
