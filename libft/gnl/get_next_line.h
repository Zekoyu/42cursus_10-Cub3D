/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 13:57:01 by mframbou          #+#    #+#             */
/*   Updated: 2021/12/14 15:44:22 by mframbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H

# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 32
# endif

# include <unistd.h>
# include <stdlib.h>
# include "../libft.h"

char	*get_next_line(int fd);
void	shift_buffer(char *buffer);
char	*get_first_line_in_string_free(char *str);
char	has_newline(char *str);
char	*ft_strcatdup(char *str1, char *str2, int str2len);

#endif