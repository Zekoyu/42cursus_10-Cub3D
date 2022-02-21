/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 13:31:04 by mframbou          #+#    #+#             */
/*   Updated: 18-02-2022 12:39 by                                             */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	remove_nl(char *line)
{
	if (line[ft_strlen(line) - 1] == '\n')
		line[ft_strlen(line) - 1] = '\0';
}

static t_texture	*get_associated_texture(char *identifier, t_game *game)
{
	if (ft_strcmp(identifier, "NO") == 0)
		return (&game->n_tex);
	else if (ft_strcmp(identifier, "SO") == 0)
		return (&game->s_tex);
	else if (ft_strcmp(identifier, "EA") == 0)
		return (&game->e_tex);
	else if (ft_strcmp(identifier, "WE") == 0)
		return (&game->w_tex);
	else
		return (NULL);
}

static int	is_xpm(char *filename)
{
	int	len;

	len = ft_strlen(filename);
	if (len >= 4)
	{
		if (filename[len - 4] == '.' \
			&& filename[len - 3] == 'x' \
			&& filename[len - 2] == 'p' \
			&& filename[len - 1] == 'm')
		{
			return (1);
		}
	}
	print_error_plus_arg("Texture is not an xpm file.\n");
	return (0);
}

/*
	Check that the texture format is valid and file is also valid
	If not an xpm, will return 1 (= error)
	else will return 0 and it's a valid file
	(or maybe not if the content is wrong but xpm
	to image will check it)
*/
static int	check_texture_line(char *line, t_game *game)
{
	char		**args;
	int			argc;
	int			fd;

	args = ft_split(line, ' ');
	if (!args)
		return (print_error_plus_arg("Encountered a ft_split error.\n"));
	argc = 0;
	while (args[argc])
		argc++;
	if (argc != 2 || !get_associated_texture(args[0], game))
	{
		free_ft_split(args);
		return (print_error_plus_arg("Wrong texture line format.\n"));
	}
	fd = open(args[1], O_RDONLY);
	if (fd == -1)
	{
		free_ft_split(args);
		return (print_error_plus_arg("No permission to open texture " \
									"file or non-existent file.\n"));
	}
	close(fd);
	return (!is_xpm(args[1]));
}

/*
	Takes a line which is considered as texture (first argument is 2 char long)
	Then try to parse it, if it's incorrect, print the error and return, else
	set it in the game struct
*/
int	parse_texture_line(char *line, t_game *game)
{
	t_texture	*texture;
	char		**args;

	remove_nl(line);
	if (check_texture_line(line, game))
	{
		free(line);
		return (-1);
	}
	else
	{
		args = ft_split(line, ' ');
		texture = get_associated_texture(args[0], game);
		texture->image.img = mlx_xpm_file_to_image(game->mlx, args[1], &texture->width, &texture->height);
		free_ft_split(args);
		if (!texture->image.img)
		{
			free(line);
			return (print_error_plus_arg("Cannot convert XPM to image\n"));
		}
	}
	free(line);
	return (0);
}

int	ft_atoi_rgb(char *str)
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
	if (num > 255)
		return (-1);
	return (num);
}

/*
	RGB format = (0-255),(0-255),(0-255)
*/
int	matches_rgb_format(char *rgb_str)
{
	char	**rgb;
	int		argc;

	rgb = ft_split(rgb_str, ',');
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

int	check_color_line(char *line)
{
	char	**args;
	int		argc;

	args = ft_split(line, ' ');
	if (!args)
		return (print_error_plus_arg("Encountered a ft_split error.\n"));
	argc = 0;
	while (args[argc])
		argc++;
	if (argc != 2)
	{
		free_ft_split(args);
		return (print_error_plus_arg("Wrong ceiling/floor color line format\n"));
	}
	if (ft_strcmp(args[0], "C") != 0 && ft_strcmp(args[0], "F") != 0)
	{
		free_ft_split(args);
		return (print_error_plus_arg("Wrong floor/ceiling identifer " \
									"(expected C or F)\n"));
	}
	if (!matches_rgb_format(args[1]))
	{
		free_ft_split(args);
		return (-1);
	}
	return (0);
}

static unsigned int	rgb_to_hex(int r, int g, int b)
{
	return (r << 16 | g << 8 | b);
}

int	parse_color_line(char *line, t_game *game)
{
	char	**args;
	char	**rgb_args;

	remove_nl(line);
	if (check_color_line(line) == -1)
		return (-1);
	args = ft_split(line, ' ');
	if (!args || !args[0])
		return (print_error_plus_arg("Encountered a ft_split error.\n"));
	rgb_args = ft_split(args[1], ',');
	if (!rgb_args || !rgb_args[0])
	{
		free_ft_split(args);
		free(line);
		return (print_error_plus_arg("Encountered a ft_split error.\n"));
	}
	if (ft_strcmp(args[0], "C") == 0)
		game->ceil_color = rgb_to_hex(ft_atoi_rgb(rgb_args[0]), \
										ft_atoi_rgb(rgb_args[1]), \
										ft_atoi_rgb(rgb_args[2]));
	else if (ft_strcmp(args[0], "F") == 0)
		game->floor_color = rgb_to_hex(ft_atoi_rgb(rgb_args[0]), \
										ft_atoi_rgb(rgb_args[1]), \
										ft_atoi_rgb(rgb_args[2]));
	free(line);
	free_ft_split(args);
	free_ft_split(rgb_args);
	return (0);
}

/*
	Lines types;
	-1 = other
	1 = floor / ceiling color
	2 = wall texture
	3 = newline (continue)
*/
int	check_line_type(char *line)
{
	int		len;
	char	**args;

	len = ft_strlen(line);
	if (len == 1 && line[0] == '\n')
		return (3);
	args = ft_split(line, ' ');
	if (!args || !args[0])
		return (print_error_plus_arg("Encountered a ft_split error.\n"));
	if (ft_strlen(args[0]) == 1)
	{
		free_ft_split(args);
		return (1);
	}
	else if (ft_strlen(args[0]) == 2)
	{
		free_ft_split(args);
		return (2);
	}
	return (print_error_plus_arg("Incorrect line found"));
}

static int	is_cub(char *filename)
{
	int	len;

	len = ft_strlen(filename);
	if (len >= 4)
	{
		if (filename[len - 4] == '.' \
			&& filename[len - 3] == 'c' \
			&& filename[len - 2] == 'u' \
			&& filename[len - 1] == 'b')
		{
			return (1);
		}
	}
	print_error_plus_arg("The given file is not a .cub file.\n");
	return (0);
}

/*
	If the current file is just a '\n', continue,
	if it's not part of the header, return because it might be part of the map
	if it's parsable but invalid, return

	return the number of lines in header
	(so that we know our first map line in the file)
*/
int	parse_cub_file_header(int fd, t_game *game)
{
	char	*line;
	int		i;

	line = get_next_line(fd);
	i = 0;
	while (line)
	{
		i++;
		if (check_line_type(line) == -1)
		{
			free(line);
			break ;
		}
		else if ((check_line_type(line) == 1 && parse_color_line(line, game) == -1) \
		|| (check_line_type(line) == 2 && parse_texture_line(line, game) == -1))
		{
			free(line);
			ft_printf("(line %d)\n\n", i);
			return (-1);
		}
		free(line);
		line = get_next_line(fd);
	}
	return (i);
}

int	are_all_values_parsed(t_game *game)
{
	int	found_error;

	found_error = 0;
	if (game->ceil_color == 0xFF000000)
		print_error_if_needed("Missing ceil color\n", &found_error);
	if (game->floor_color == 0xFF000000)
		print_error_if_needed("Missing ceil color\n", &found_error);
	if (game->n_tex.image.img == NULL)
		print_error_if_needed("Missing ceil color\n", &found_error);
	if (game->s_tex.image.img == NULL)
		print_error_if_needed("Missing ceil color\n", &found_error);
	if (game->e_tex.image.img == NULL)
		print_error_if_needed("Missing ceil color\n", &found_error);
	if (game->w_tex.image.img == NULL)
		print_error_if_needed("Missing ceil color\n", &found_error);
	return (!found_error);
}

/*
	Check if line only has the valid characters
*/
static int	is_line_valid(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (!(line[i] == '0' || line[i] == '1' || line[i] == ' ' \
		|| line[i] == 'N' || line[i] == 'S' || line[i] == 'E' \
		|| line[i] == 'W'))
			return (0);
		i++;
	}
	return (1);
}


/*
	Dont free map[i] since it's copied into the new one
	Only free double array base
*/
char	**ft_addstr_to_str_array(char **map, char *new_line)
{
	int		i;
	char	**new;

	i = 0;
	while (map[i])
		i++;
	new = malloc(sizeof(char *) * (i + 2));
	if (!new)
		return (NULL);
	i = 0;
	while (map[i])
	{
		new[i] = map[i];
		i++;
	}
	new[i++] = new_line;
	new[i] = NULL;
	free(map);
	return (new);
}

/*
	Converts everything to 0 and 1 except player pos
*/
char	*convert_line_to_map_line(char *line)
{
	char	*res;
	int		i;

	res = malloc(sizeof(char) * (ft_strlen(line) + 1));
	if (!res)
		return (NULL);
	i = 0;
	while (line[i])
	{
		if (line[i] == ' ' || line[i] == '0')
			res[i] = '0';
		else if (line[i] == '1')
			res[i] = '1';
		else if (line[i] == 'N' || line[i] == 'S' || line[i] == 'E' \
		|| line[i] == 'W')
			res[i] = line[i];
		else
		{
			free(res);
			return (NULL);
		}
		i++;
	}
	return (res);
}

/*
	Realloc the line and realloc the map to add a line
	So we can free the line in our calling functio

/*
	We can do 2 remove_nl on the same line
	because we know that GNL will return on the 1st '\n'encountered
	so it shouldn't remove 2 '\n' on the line (which we don't want)
	since there is only 0 or 1
	(line Test\n\n => Test\n => Test is impossible)
*/
int	parse_cub_file_map(int fd, t_game *game)
{
	char	*line;
	int		width;
	int		height;
	char	**map;

	line = get_next_line(fd);
	if (line)
	{
		remove_nl(line);
		width = ft_strlen(line);
	}
	while (line)
	{
		remove_nl(line);

		if (ft_strlen(line) != width)
		{
			// return error
		}

		free(line);
		line = get_next_line(fd);
	}
	return (0);
}

/*
	Parse header, then if valid reopen file and go
	to the potential first line of map (returned by parse_cub_file_header).

	We do it simply by calling GNL x times
	(while freeing it directly since we don't need the value)
*/
int	parse_cub_file(char *filename, t_game *game)
{
	int	map_start_line;
	int	fd;

	if (!is_cub(filename))
		return (-1);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (print_error_plus_arg("Cannot open the .cub file (non-existent or no permission)\n"));
	map_start_line = parse_cub_file_header(fd, game);
	close(fd);
	if (!are_all_values_parsed(game))
		return (-1);
	if (map_start_line == -1)
		return (-1);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (print_error_plus_arg("Cannot re-open the .cub file\n"));
	while (map_start_line--)
		free(get_next_line(fd));
	if (parse_cub_file_map(fd, game) == -1)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

/*
int main(int argc, char *argv[])
{
	t_game	game;
	int		test = 0;

	game.ceil_color = 0xFF000000;
	game.floor_color = 0xFF000000;
	game.n_tex.image.img = NULL;
	game.e_tex.image.img = NULL;
	game.s_tex.image.img = NULL;
	game.w_tex.image.img = NULL;
	

	game.mlx = mlx_init();
	if (argc >= 2)
		test = parse_cub_file(argv[1], &game);
	printf("Returned value: %d\n", test);
}
*/
