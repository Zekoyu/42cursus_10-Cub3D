/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 12:56:57 by mframbou          #+#    #+#             */
/*   Updated: 2021/12/14 15:45:04 by mframbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

// Concatenate 2 strings and puts the result in a new string
// Then frees the original string (if it's null, it doesn't do it, source: man)
char	*ft_strcatdup(char *str1, char *str2, int str2len)
{
	int		i;
	int		j;
	char	*res;

	if (!str1 || !str2)
		return (NULL);
	i = 0;
	while (str1[i])
		i++;
	res = malloc(sizeof(char) * (i + str2len + 1));
	if (res)
	{
		j = str2len;
		res[i + j] = '\0';
		while (--j >= 0)
			res[i + j] = str2[j];
		while (--i >= 0)
			res[i] = str1[i];
	}
	free(str1);
	return (res);
}

char	has_newline(char *str)
{
	while (*str)
		if (*str++ == '\n')
			return (1);
	return (0);
}

char	*get_first_line_in_string_free(char *str)
{
	int		i;
	char	*res;

	if (!str)
		return (NULL);
	i = 0;
	while (str[i] != '\n' && str[i])
		i++;
	if (str[i] == '\n')
		i++;
	res = malloc (sizeof (char) * (i + 1));
	res[i--] = '\0';
	while (i >= 0)
	{
		res[i] = str[i];
		i--;
	}
	free(str);
	return (res);
}

void	shift_buffer(char *buffer)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (buffer[i] != '\n' && buffer[i])
		i++;
	if (buffer[i] == '\n')
	{
		i++;
		while (buffer[i])
			buffer[j++] = buffer[i++];
		while (buffer[j])
			buffer[j++] = '\0';
	}
	else
	{
		while (buffer[j])
			buffer[j++] = '\0';
	}
}
