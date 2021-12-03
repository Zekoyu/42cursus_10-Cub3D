/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/06 16:37:47 by mframbou          #+#    #+#             */
/*   Updated: 2021/08/08 20:04:53 by mframbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdlib.h>
#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*substr;
	size_t	i;

	if (start >= ft_strlen(s))
	{
		substr = (char *) malloc(sizeof (char));
		if (substr)
			*substr = '\0';
		return (substr);
	}
	substr = (char *) malloc(sizeof(char) * (len + 1));
	if (substr)
	{
		i = 0;
		while (i < len && s[i])
			substr[i++] = s[start++];
		substr[i] = '\0';
	}
	return (substr);
}
