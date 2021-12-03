/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/06 16:26:29 by mframbou          #+#    #+#             */
/*   Updated: 2021/08/06 16:29:22 by mframbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*fresh;
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	fresh = (char *) malloc (sizeof (char) * (i + 1));
	if (fresh)
	{
		fresh[i] = '\0';
		while (i-- > 0)
			fresh[i] = (f)(i, s[i]);
	}
	return (fresh);
}
