/* ************************************************************************** */
/*                                                                            */
/*                                  .-.                       .               */
/*                                 / -'                      /                */
/*                  .  .-. .-.   -/--).--..-.  .  .-. .-.   /-.  .-._.)  (    */
/*   By:             )/   )   )  /  /    (  |   )/   )   ) /   )(   )(    )   */
/*                  '/   /   (`.'  /      `-'-''/   /   (.'`--'`-`-'  `--':   */
/*   Created: 23-02-2022  by  `-'                        `-'                  */
/*   Updated: 23-02-2022 15:48 by                                             */
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
static int	check_map(int fd, int *width, int *height)
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

/*
	+5 for secret room
*/
static int	**malloc_map(int width, int height)
{
	int		x;
	int		y;
	int		**map;

	x = 0;
	y = 0;
	map = (int **) malloc(sizeof(int *) * height);
	if (!map)
		return (NULL);
	while (y < height)
	{
		map[y] = (int *) malloc(sizeof(int) * (width + 5));
		ft_bzero(map[y], sizeof(int) * (width + 5));
		if (!map[y])
		{
			while (--y >= 0)
				free(map[y]);
			return (NULL);
		}
		y++;
	}
	return (map);
}

static void	fill_map(int **map, int height, int width, int fd)
{
	char	*line;
	int		x;
	int		y;
	int		ln_len;

	line = get_next_line(fd);
	while (line)
	{	
		remove_nl(line);
		x = 0;
		ln_len = ft_strlen(line);
		while (x < ln_len)
		{
			if (line[x] == '1')
				map[y][x] = 1;
			else if (line[x] == 'N' || line[x] == 'S' || line[x] == 'E' \
			|| line[x] == 'W')
				map[y][x] = get_face_player(line[x]);
			x++;
		}
		free(line);
		line = get_next_line(fd);
		y++;
	}
}

/*
	- Left wall
	- Right wall
	- Top wall
	- Bottom wall
*/
static void	fill_secret_room(int **map, int height, int width)
{
	int	x;
	int	y;

	x = width;
	y = 0;
	while (y < height)
		map[y++][x] = 1;
	x = width + 4;
	y = 0;
	while (y < height)
		map[y++][x] = 1;
	x = width;
	y = 0;
	while (x < width + 5)
		map[y][x++] = 1;
	x = width;
	y = height - 1;
	while (x < width + 5)
		map[y][x++] = 1;
}

int	parse_map(char *filename, int start_line, t_game *game)
{
	int		width;
	int		height;
	int		**map;
	int		fd;

	fd = read_until_line(filename, start_line);
	if (fd == -1)
		return (print_error_plus_arg("Couldn't reopen the .cub file.\n"));
	if (check_map(fd, &width, &height) == -1)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	if (fd == -1)
		return (print_error_plus_arg("Couldn't re-reopen the .cub file.\n"));
	map = malloc_map(width, height);
	if (!map)
		return (print_error_plus_arg("Couldn't malloc map.\n"));
	flush_gnl_buffer();
	fd = read_until_line(filename, start_line);
	fill_map(map, height, width, fd);
	fill_secret_room(map, height, width);
	if (set_player_pos_and_dir(&(game->player), map, width, height) == -1)
		return (-1);
	game->map.map = map;
	game->map.height = height;
	game->map.width = width;
	game->map.total_height = height;
	game->map.total_width = width + 5;
	return (0);
}
