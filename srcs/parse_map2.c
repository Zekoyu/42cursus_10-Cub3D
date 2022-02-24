/* ************************************************************************** */
/*                                                                            */
/*                                  .-.                       .               */
/*                                 / -'                      /                */
/*                  .  .-. .-.   -/--).--..-.  .  .-. .-.   /-.  .-._.)  (    */
/*   By:             )/   )   )  /  /    (  |   )/   )   ) /   )(   )(    )   */
/*                  '/   /   (`.'  /      `-'-''/   /   (.'`--'`-`-'  `--':   */
/*   Created: 23-02-2022  by  `-'                        `-'                  */
/*   Updated: 24-02-2022 15:52 by                                             */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
	Found nl is true when an empty line is encoutered, if we have stuff
	after it, it's an error
*/
static int	check_one_line_map(char *line, int *width, int *height)
{
	int			line_length;
	static int	found_nl = 0;

	if (check_line_type(line) == INVALID_LINE)
		return (-1);
	else if (check_line_type(line) == EMPTY_LINE)
		found_nl = 1;
	else if (found_nl)
		return (print_error_plus_arg("Found stuff after map.\n"));
	(*height)++;
	line_length = ft_strlen(line);
	if (line_length > *width)
		*width = line_length;
	if (!is_line_valid_in_map(line))
		return (print_error_plus_arg("Invalid character in map.\n"));
	return (0);
}

/*
	Check map validity, set width and height
	Return -1 on error (eg. found_nl (found stuff after map))
*/
int	check_map(int fd, int *width, int *height)
{
	char	*line;
	int		i;
	int		line_length;

	*height = 0;
	*width = 0;
	line = get_next_line(fd);
	while (line)
	{
		remove_nl(line);
		if (check_one_line_map(line, width, height) == -1)
		{
			free(line);
			return (-1);
		}
		free(line);
		line = get_next_line(fd);
	}
	return (0);
}
