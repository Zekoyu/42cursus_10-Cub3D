/* ************************************************************************** */
/*                                                                            */
/*                                  .-.                       .               */
/*                                 / -'                      /                */
/*                  .  .-. .-.   -/--).--..-.  .  .-. .-.   /-.  .-._.)  (    */
/*   By:             )/   )   )  /  /    (  |   )/   )   ) /   )(   )(    )   */
/*                  '/   /   (`.'  /      `-'-''/   /   (.'`--'`-`-'  `--':   */
/*   Created: 23-02-2022  by  `-'                        `-'                  */
/*   Updated: 23-02-2022 17:23 by                                             */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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

static int	parse_map2(int **map, int height, int width, t_game *game)
{
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
	map = malloc_map(width, height);
	if (!map)
		return (print_error_plus_arg("Couldn't malloc map.\n"));
	flush_gnl_buffer();
	fd = read_until_line(filename, start_line);
	if (fd == -1)
		return (print_error_plus_arg("Couldn't re-reopen the .cub file.\n"));
	fill_map(map, height, width, fd);
	close (fd);
	return (parse_map2(map, height, width, game));
}
