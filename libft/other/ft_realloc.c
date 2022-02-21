/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/17 17:47:31 by mframbou          #+#    #+#             */
/*   Updated: 2021/12/17 17:52:05 by mframbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	*ft_realloc(void *mem, size_t mem_size, size_t new_bytes_num)
{
	void	*res;
	int		i;

	res = malloc(mem_size + new_bytes_num);
	i = 0;
	while (i < mem_size)
	{
		((char *)res)[i] = ((char *)mem)[i];
		i++;
	}
	return (res);
}
