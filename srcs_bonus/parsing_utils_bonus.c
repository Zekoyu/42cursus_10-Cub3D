/* ************************************************************************** */
/*                                                                            */
/*                                  .-.                       .               */
/*                                 / -'                      /                */
/*                  .  .-. .-.   -/--).--..-.  .  .-. .-.   /-.  .-._.)  (    */
/*   By:             )/   )   )  /  /    (  |   )/   )   ) /   )(   )(    )   */
/*                  '/   /   (`.'  /      `-'-''/   /   (.'`--'`-`-'  `--':   */
/*   Created: 23-02-2022  by  `-'                        `-'                  */
/*   Updated: 25-02-2022 12:36 by                                             */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

int	read_until_line(char *file, int line)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd != -1)
	{
		while (--line > 0)
			free(get_next_line(fd));
	}
	return (fd);
}

void	remove_nl(char *line)
{
	if (line[ft_strlen(line) - 1] == '\n')
		line[ft_strlen(line) - 1] = '\0';
}

#ifndef DO_BONUSES

int	is_valid_char_in_map(char c)
{
	return (c == '0' || c == '1' || c == ' ' || \
			c == 'N' || c == 'S' || c == 'E' || c == 'W');
}
#else

int	is_valid_char_in_map(char c)
{
	return (c == '0' || c == '1' || c == ' ' || \
			c == 'N' || c == 'S' || c == 'E' || c == 'W' || \
			c == 'D');
}
#endif

/*
	Check if line only has the valid characters
*/
int	is_line_valid_in_map(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (!is_valid_char_in_map(line[i]))
			return (0);
		i++;
	}
	return (1);
}
