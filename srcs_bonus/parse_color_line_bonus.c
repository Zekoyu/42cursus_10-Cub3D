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

static int	ft_atoi_rgb(char *str)
{
	int	i;
	int	num;

	num = 0;
	i = 0;
	while (ft_isdigit(str[i]) && str[i] != 0)
	{
		num = num * 10 + (str[i] - 48);
		i++;
		if (i > 3)
			return (-1);
	}
	if (str[i] || num > 255)
		return (-1);
	return (num);
}

/*
	RGB format = (0-255),(0-255),(0-255)
*/
static int	matches_rgb_format(char *rgb_str)
{
	char	**rgb;
	int		argc;

	rgb = ft_split(rgb_str, ",");
	if (!rgb || !rgb[0])
	{
		print_error_plus_arg("Wrong rgb format (expected R,G,B)\n");
		return (0);
	}
	argc = 0;
	while (rgb[argc])
		argc++;
	if (argc != 3)
	{
		print_error_plus_arg("Wrong RGB format (expected R,G,B)\n");
		return (0);
	}
	if (ft_atoi_rgb(rgb[0]) == -1 || ft_atoi_rgb(rgb[1]) == -1 \
		|| ft_atoi_rgb(rgb[2]) == -1)
	{
		print_error_plus_arg("Wrong RGB format (expected R,G,B)\n");
		return (0);
	}
	return (1);
}

static int	check_color_line(char *line)
{
	char	**args;
	int		argc;

	args = ft_split(line, " ");
	if (!args)
		return (print_error_plus_arg("Encountered a ft_split error.\n"));
	argc = 0;
	while (args[argc])
		argc++;
	if (argc != 2)
		ft_free_str_array(args);
	if (argc != 2)
		return (print_error_plus_arg("Wrong ceiling/floor color line format\n"));
	if (ft_strcmp(args[0], "C") != 0 && ft_strcmp(args[0], "F") != 0)
	{
		ft_free_str_array(args);
		return (print_error_plus_arg("Wrong floor/ceiling identifer " \
									"(expected C or F)\n"));
	}
	if (!matches_rgb_format(args[1]))
	{
		ft_free_str_array(args);
		return (-1);
	}
	return (0);
}

/*
unsigned int	rgb_to_hex(int r, int g, int b)
{
	return (r << 16 | g << 8 | b);
}
*/
int	set_game_color(t_game *game, char *identifier, char **rgb)
{
	if (ft_strcmp(identifier, "C") == 0)
	{
		if ((unsigned int) game->ceil_color != 0xFF000000)
		{
			return (print_error_plus_arg("Ceiling color found more \
										than once.\n"));
		}
		game->ceil_color = (ft_atoi_rgb(rgb[0]) << 16 \
							| ft_atoi_rgb(rgb[1]) << 8 \
							| ft_atoi_rgb(rgb[2]));
	}
	else if (ft_strcmp(identifier, "F") == 0)
	{
		if ((unsigned int) game->floor_color != 0xFF000000)
		{
			return (print_error_plus_arg("Floor color found more than once.\n"));
		}
		game->floor_color = (ft_atoi_rgb(rgb[0]) << 16 \
							| ft_atoi_rgb(rgb[1]) << 8 \
							| ft_atoi_rgb(rgb[2]));
	}
	return (0);
}

int	parse_color_line(char *line, t_game *game)
{
	char	**args;
	char	**rgb_args;

	if (check_color_line(line) == -1)
		return (-1);
	args = ft_split(line, " ");
	if (!args || !args[0])
		return (print_error_plus_arg("Encountered a ft_split error.\n"));
	rgb_args = ft_split(args[1], ",");
	if (!rgb_args || !rgb_args[0])
	{
		ft_free_str_array(args);
		free(line);
		return (print_error_plus_arg("Encountered a ft_split error.\n"));
	}
	if (set_game_color(game, args[0], rgb_args) == -1)
	{
		free(line);
		ft_free_str_array(rgb_args);
		ft_free_str_array(args);
		return (-1);
	}
	ft_free_str_array(args);
	ft_free_str_array(rgb_args);
	return (0);
}
