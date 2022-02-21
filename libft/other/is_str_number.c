/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_str_number.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 17:16:51 by mframbou          #+#    #+#             */
/*   Updated: 2021/12/15 10:47:14 by mframbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	is_str_number(char *number_str)
{
	int	i;

	i = 0;
	while (number_str[i])
		if (!ft_isdigit(number_str[i++]))
			return (0);
	return (1);
}
