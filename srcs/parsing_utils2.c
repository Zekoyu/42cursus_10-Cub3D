/* ************************************************************************** */
/*                                                                            */
/*                                  .-.                       .               */
/*                                 / -'                      /                */
/*                  .  .-. .-.   -/--).--..-.  .  .-. .-.   /-.  .-._.)  (    */
/*   By:             )/   )   )  /  /    (  |   )/   )   ) /   )(   )(    )   */
/*                  '/   /   (`.'  /      `-'-''/   /   (.'`--'`-`-'  `--':   */
/*   Created: 23-02-2022  by  `-'                        `-'                  */
/*   Updated: 23-02-2022 15:37 by                                             */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
	Lines types;
	-1 = unknown
	1 = floor / ceiling color
	2 = wall texture
	3 = newline (continue)
*/
int	check_line_type(char *line)
{
	int		len;
	char	**args;

	len = ft_strlen(line);
	if (len == 0)
		return (EMPTY_LINE);
	args = ft_split(line, " ");
	if (!args || !args[0])
		return (print_error_plus_arg("Encountered a ft_split error.\n"));
	if (ft_strlen(args[0]) == 1)
	{
		ft_free_str_array(args);
		return (COLOR_LINE);
	}
	else if (ft_strlen(args[0]) == 2)
	{
		ft_free_str_array(args);
		return (TEXTURE_LINE);
	}
	else if (is_line_valid_in_map(line))
	{
		ft_free_str_array(args);
		return (MAP_LINE);
	}
	return (print_error_plus_arg("Incorrect line found.\n"));
}
