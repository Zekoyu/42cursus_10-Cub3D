/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_ft_split.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 15:48:13 by mframbou          #+#    #+#             */
/*   Updated: 2021/12/14 15:50:28 by mframbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	free_ft_split(char **ft_split_return)
{
	int	i;

	i = 0;
	while (ft_split_return[i])
		free(ft_split_return[i++]);
	free(ft_split_return);
}
