/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/06 17:15:28 by mframbou          #+#    #+#             */
/*   Updated: 2021/11/02 11:36:14 by mframbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

static void	*clean_array(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		free(arr[i++]);
	return (NULL);
}

static int	get_substring_count(char const *str, char c)
{
	int		str_count;
	size_t	i;

	str_count = 0;
	i = 0;
	while (str[i])
	{
		i++;
		while (str[i] != c && str[i] != '\0')
			i++;
		if (i != 0 && str[i - 1] != c)
			str_count++;
	}
	return (str_count);
}

char	**ft_split(char const *s, char c)
{
	char	**res;
	int		substr_count;
	int		i;
	size_t	j;
	size_t	substr_start;

	i = -1;
	j = 0;
	substr_count = get_substring_count(s, c);
	res = malloc (sizeof (char *) * (substr_count + 1));
	if (!res)
		return (NULL);
	while (++i < substr_count)
	{
		while (s[j] == c && s[j] != '\0')
			j++;
		substr_start = j;
		while (s[j] != c && s[j] != '\0')
			j++;
		res[i] = ft_substr(s, substr_start, (j - substr_start));
		if (!res[i])
			return (clean_array(res));
	}
	res[i] = 0;
	return (res);
}
