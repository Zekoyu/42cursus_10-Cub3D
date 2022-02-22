/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mframbou <mframbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 13:31:04 by mframbou          #+#    #+#             */
/*   Updated: 22-02-2022 16:08 by                                             */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

#define INVALID_LINE -1
#define COLOR_LINE 1
#define TEXTURE_LINE 2
#define EMPTY_LINE 3
#define MAP_LINE 4

static int	read_until_line(char *file, int line)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd != -1)
	{
		int i = 0;
		while (--line > 0)
			free(get_next_line(fd));
	}
	return (fd);
}

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


static int	is_valid_char_in_map(char c)
{
	return (c == '0' || c == '1' || c == ' ' || \
			c == 'N' || c =='S' || c == 'E' || c == 'W');
}

/*
	Check if line only has the valid characters
*/
static int	is_line_valid_in_map(char *line)
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

	args = ft_split(line, " ");
	if (!args)
		return (print_error_plus_arg("Encountered a ft_split error.\n"));
	argc = 0;
	while (args[argc])
		argc++;
	if (argc != 2 || !get_associated_texture(args[0], game))
	{
		ft_free_str_array(args);
		return (print_error_plus_arg("Wrong texture line format.\n"));
	}
	fd = open(args[1], O_RDONLY);
	if (fd == -1)
	{
		ft_free_str_array(args);
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

	if (check_texture_line(line, game))
	{
		free(line);
		return (-1);
	}
	else
	{
		args = ft_split(line, " ");
		texture = get_associated_texture(args[0], game);
		if (texture->image.img)
		{
			free(line);
			return (print_error_plus_arg("Found a texture more than once.\n"));
		}
		texture->image.img = mlx_xpm_file_to_image(game->mlx, args[1], &texture->width, &texture->height);
		ft_free_str_array(args);
		if (!texture->image.img)
		{
			free(line);
			return (print_error_plus_arg("Cannot convert XPM to image\n"));
		}
	}
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
	if (str[i] || num > 255)
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

int	check_color_line(char *line)
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
	{
		ft_free_str_array(args);
		return (print_error_plus_arg("Wrong ceiling/floor color line format\n"));
	}
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

static unsigned int	rgb_to_hex(int r, int g, int b)
{
	return (r << 16 | g << 8 | b);
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
	if (ft_strcmp(args[0], "C") == 0)
	{
		if (game->ceil_color != 0xFF000000)
		{
			ft_free_str_array(args);
			free(line);
			return (print_error_plus_arg("Ceiling color found more than once.\n"));
		}
		game->ceil_color = rgb_to_hex(ft_atoi_rgb(rgb_args[0]), \
										ft_atoi_rgb(rgb_args[1]), \
										ft_atoi_rgb(rgb_args[2]));
	}
	else if (ft_strcmp(args[0], "F") == 0)
	{
		if (game->floor_color != 0xFF000000)
		{
			ft_free_str_array(args);
			free(line);
			return (print_error_plus_arg("Floor color found more than once.\n"));
		}
		game->floor_color = rgb_to_hex(ft_atoi_rgb(rgb_args[0]), \
										ft_atoi_rgb(rgb_args[1]), \
										ft_atoi_rgb(rgb_args[2]));
	}
	ft_free_str_array(args);
	ft_free_str_array(rgb_args);
	return (0);
}

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
		remove_nl(line);
		i++;
		if (check_line_type(line) == INVALID_LINE)
		{
			ft_printf("(line %d)\n\n", i);
			free(line);
			return (-1);
		}
		else if ((check_line_type(line) == COLOR_LINE && parse_color_line(line, game) == -1) \
		|| (check_line_type(line) == TEXTURE_LINE && parse_texture_line(line, game) == -1))
		{
			// line already freed if error
			ft_printf("(line %d)\n\n", i);
			return (-1);
		}
		else if (check_line_type(line) == MAP_LINE)
		{
			free(line);
			return (i);
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
	Check map validity, set width and height
	Return -1 on error
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
		(*height)++;
		line_length = ft_strlen(line);
		printf("line = \"%s\"\n", line);
		if (line_length > *width)
			*width = line_length;
		i = 0;
		while (i < line_length)
		{
			if (!is_valid_char_in_map(line[i]))
			{
				free(line);
				return (print_error_plus_arg("Invalid character in map.\n"));
			}
			i++;
		}
		free(line);
		line = get_next_line(fd);
	}
	return (0);
}

int	parse_cub_file_map(char *filename, int start_line, t_game *game)
{
	int		width;
	int		height;
	char	**map;
	int		fd;

	printf("Start line: %d\n", start_line);
	fd = read_until_line(filename, start_line);
	
	
	if (fd == -1)
		return (print_error_plus_arg("Couldn't reopen the .cub file.\n"));
	if (check_map(fd, &width, &height) == -1)
	{
		close(fd);
		return (-1);
	}
	printf("Map width: %d, height: %d\n", width, height);
	return (0);
}

/*
int    parse_cub_file_map(int fd, char *file, t_cube *data, int nb_of_line_before)
{
	char 	*line;
    int      max_line_lenght;
    char    **map;
	int 	nb_of_lines; 


 // ---------VERIFY THE MAP --------- (First pass)
 	nb_of_lines = 0;
	line = get_next_line(fd);
	if(!line)
		return 0;
	max_line_lenght = 0;
	while(line)
	{
		remove_newline(line);
		int i;
		int line_lenght;

		i = 0;
		if (line[i] == 0) // line is empty
			return 0;
		nb_of_lines++;
		
		line_lenght = ft_strlen(line);

		if (max_line_lenght < line_lenght)
			max_line_lenght = line_lenght;
		
		while(i < line_lenght)
			if(!is_valid_char(line[i++]))
				return 0;
		free(line);
		line = get_next_line(fd);
		
		
	}
	if (nb_of_lines < 3) // has to be minimum 3 lines for a valid map
		return 0;

	close(fd);
	fd  = open(file, O_RDONLY);

	
	
 // ---------PARSE THE MAP --------- (second pass)
 	int k;
	k = 0;
	while(k++ < nb_of_line_before) // go to the begining of the map
		free(get_next_line(fd));
	int x;
	int y;
	x = 0;
	y = 0;
	map = (char **)malloc(sizeof(char *) * nb_of_lines);
	while(x < nb_of_lines)
	{
		map[x] = (char *)malloc(sizeof(char) * (max_line_lenght));
		memset(map[x],0,max_line_lenght);
		x++;
	}
		
	line = get_next_line(fd);
	
	while(line)
	{	
		remove_newline(line);
		x = 0;
		while(x < strlen(line))
		{
			if (line[x] == ' ')
				map[y][x] = 0;
			else
				map[y][x] = line[x] - '0';
			x++;
		}
		free(line);
		line = get_next_line(fd);
		y++;
	}

	
	for (int j = 0; j < nb_of_lines; j++)
	{
		for (int i = 0; i < max_line_lenght; i++)
		{
			printf("%d", map[j][i]);
		}
		printf("\n");
	}
	return 1;
}
*/

/*
	Read file until the given line (until line 1 would read 0 lines)
	return the fd / -1 on error

	line parameter will be the next one read by get_next_line
*/

/*
	Parse header, then if valid reopen file and go
	to the potential first line of map (returned by parse_cub_file_header).

	We do it simply by calling GNL x times
	(while freeing it directly since we don't need the value)

	flush buffer after first close since we close in the middle of file
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
	map_start_line = parse_cub_file_header(fd, game); // since we already read the first line using gnl, we need to go back to it
	if (map_start_line == -1 || !are_all_values_parsed(game))
	{
		close(fd);
		return (-1);
	}
	close(fd);
	flush_gnl_buffer();
	if (parse_cub_file_map(filename, map_start_line, game) == -1)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}


#include <stdio.h>
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
	
/*
	int fd = read_until_line("srcs/pouet.test", 9);
	printf("gnl read line: \"%s\"\n", get_next_line(fd));
	close(fd);*/
}

