/* ************************************************************************** */
/*                                                                            */
/*                                  .-.                       .               */
/*                                 / -'                      /                */
/*                  .  .-. .-.   -/--).--..-.  .  .-. .-.   /-.  .-._.)  (    */
/*   By:             )/   )   )  /  /    (  |   )/   )   ) /   )(   )(    )   */
/*                  '/   /   (`.'  /      `-'-''/   /   (.'`--'`-`-'  `--':   */
/*   Created: 23-02-2022  by  `-'                        `-'                  */
/*   Updated: 25-02-2022 15:21 by                                             */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

#ifndef DO_BONUSES

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
		map[y] = (int *) malloc(sizeof(int) * (width));
		ft_bzero(map[y], sizeof(int) * (width));
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

static void	put_walls_around_the_map_so_we_dont_crash_the_game(int **map, \
														int width, int height)
{
	int	x;
	int	y;

	x = 0;
	y = 0;
	while (x < width)
		map[y][x++] = 1;
	x = 0;
	y = height - 1;
	while (x < width)
		map[y][x++] = 1;
	x = 0;
	while (y < height)
		map[y++][x] = 1;
	x = width - 1;
	y = 0;
	while (y < height)
		map[y++][x] = 1;
}

static void	fill_map(int **map, int fd)
{
	char	*line;
	int		x;
	int		y;
	int		ln_len;

	line = get_next_line(fd);
	y = 0;
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

static int	parse_map2(int **map, int height, int width, t_game *game)
{
	t_point	player_pt;

	if (set_player_pos_and_dir(&(game->player), map, width, height) == -1)
		return (-1);
	game->map.map = map;
	game->map.height = height;
	game->map.width = width;
	game->map.total_height = height;
	game->map.total_width = width;
	player_pt.x = (int) game->player.pos.x;
	player_pt.y = (int) game->player.pos.y;
	if (!is_enclosed(map, width, height, player_pt))
		return (print_error_plus_arg("Map not enclosed.\n"));
	put_walls_around_the_map_so_we_dont_crash_the_game(map, width, height);
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
	fill_map(map, fd);
	close (fd);
	return (parse_map2(map, height, width, game));
}
#endif