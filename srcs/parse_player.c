/* ************************************************************************** */
/*                                                                            */
/*                                  .-.                       .               */
/*                                 / -'                      /                */
/*                  .  .-. .-.   -/--).--..-.  .  .-. .-.   /-.  .-._.)  (    */
/*   By:             )/   )   )  /  /    (  |   )/   )   ) /   )(   )(    )   */
/*                  '/   /   (`.'  /      `-'-''/   /   (.'`--'`-`-'  `--':   */
/*   Created: 23-02-2022  by  `-'                        `-'                  */
/*   Updated: 23-02-2022 15:25 by                                             */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

enum e_player_dir
{
	FACE_NORTH = 123,
	FACE_SOUTH,
	FACE_EAST,
	FACE_WEST
};

/*
	+------> X
	|
	|
  Y	|
  	V

	To face north, need to face negative Y
	south = positive Y etc.

	default fov is 90 degrees (same direction and cam_plane)
	direction of 0.8 means that if we are closer than 0.8 from wall, it will
	take the whole screen

	init to 0 at start for norminette to gain few lines
*/
static void	face_player_dir(t_player *player, int dir)
{
	player->direction.x = 0.0;
	player->direction.y = 0.0;
	player->cam_plane.x = 0.0;
	player->cam_plane.y = 0.0;
	if (dir == FACE_SOUTH)
	{
		player->direction.y = 1.0;
		player->cam_plane.x = -1.0;
	}
	else if (dir == FACE_NORTH)
	{
		player->direction.y = -1.0;
		player->cam_plane.x = 1.0;
	}
	else if (dir == FACE_WEST)
	{
		player->direction.x = -1.0;
		player->cam_plane.y = -1.0;
	}
	else if (dir == FACE_EAST)
	{
		player->direction.x = 1.0;
		player->cam_plane.y = 1.0;
	}
}

static inline void	set_player_pos(t_player *player, float x, float y)
{
	player->pos.x = x;
	player->pos.y = y;
}

int	get_face_player(char c)
{
	if (c == 'N')
		return (FACE_NORTH);
	else if (c == 'S')
		return (FACE_SOUTH);
	else if (c == 'E')
		return (FACE_EAST);
	else if (c == 'W')
		return (FACE_WEST);
	return (-1);
}

/*
	Set player pos in t_game, return -1 on error (no player / multiple players)
*/
int	set_player_pos_and_dir(t_player *player, int **map, int width, int height)
{
	int	x;
	int	y;

	y = -1;
	player->pos.x = -1.0;
	player->pos.y = -1.0;
	while (++y < height)
	{
		x = -1;
		while (++x < width)
		{
			if (map[y][x] == FACE_NORTH || map[y][x] == FACE_SOUTH \
			|| map[y][x] == FACE_EAST || map[y][x] == FACE_WEST)
			{
				if (player->pos.x != -1.0 && player->pos.y != -1.0)
					return (print_error_plus_arg("More than 1 player found.\n"));
				set_player_pos(player, (float) x + 0.5, (float) y + 0.5);
				face_player_dir(player, map[y][x]);
				map[y][x] = 0;
			}
		}
	}
	if (player->pos.x != -1.0 && player->pos.y != -1.0)
		return (0);
	return (print_error_plus_arg("No player in the map.\n"));
}
