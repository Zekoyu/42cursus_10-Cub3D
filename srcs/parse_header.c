/* ************************************************************************** */
/*                                                                            */
/*                                  .-.                       .               */
/*                                 / -'                      /                */
/*                  .  .-. .-.   -/--).--..-.  .  .-. .-.   /-.  .-._.)  (    */
/*   By:             )/   )   )  /  /    (  |   )/   )   ) /   )(   )(    )   */
/*                  '/   /   (`.'  /      `-'-''/   /   (.'`--'`-`-'  `--':   */
/*   Created: 23-02-2022  by  `-'                        `-'                  */
/*   Updated: 23-02-2022 15:38 by                                             */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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

	line = get_next_line(fd);
	i = 0;
	while (line)
	{
		remove_nl(line);
		i++;
		line_type = check_line_type(line);
		if (line_type == INVALID_LINE)
		{
			ft_printf("(line %d)\n\n", i);
			free(line);
			return (-1);
		}
		else if ((line_type == COLOR_LINE && parse_color_line(line, game) == -1) \
		|| (line_type == TEXTURE_LINE && parse_texture_line(line, game) == -1))
		{
			ft_printf("(line %d)\n\n", i);
			return (-1);
		}
		else if (line_type == MAP_LINE)
		{
			free(line);
			return (i);
		}
		free(line);
		line = get_next_line(fd);
	}
	return (i);
}
