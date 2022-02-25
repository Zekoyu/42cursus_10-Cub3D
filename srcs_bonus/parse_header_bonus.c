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

/*
	-1 = failure
	0 = success
	> 0 = map start line
*/
int	check_header_line(char *line, t_game *game, int current_index)
{
	int	line_type;

	line_type = check_line_type(line);
	if (line_type == INVALID_LINE)
	{
		printf("(line %d)\n\n", current_index);
		free(line);
		return (-1);
	}
	else if ((line_type == COLOR_LINE && parse_color_line(line, game) == -1) \
	|| (line_type == TEXTURE_LINE && parse_texture_line(line, game) == -1))
	{
		printf("(line %d)\n\n", current_index);
		return (-1);
	}
	else if (line_type == MAP_LINE)
	{
		free(line);
		return (current_index);
	}
	return (0);
}

/*
	If the current file is just a '\n', continue,
	if it's not part of the header, return because it might be part of the map
	if it's parsable but invalid, return

	return the number of lines in header
	(so that we know our first map line in the file)
*/
int	parse_header(int fd, t_game *game)
{
	char	*line;
	int		i;
	int		line_type;
	int		checked_line;

	line = get_next_line(fd);
	i = 0;
	while (line)
	{
		remove_nl(line);
		i++;
		line_type = check_line_type(line);
		checked_line = check_header_line(line, game, i);
		if (checked_line == -1)
			return (-1);
		else if (checked_line > 0)
			return (checked_line);
		free(line);
		line = get_next_line(fd);
	}
	return (i);
}
