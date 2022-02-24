/* ************************************************************************** */
/*                                                                            */
/*                                  .-.                       .               */
/*                                 / -'                      /                */
/*                  .  .-. .-.   -/--).--..-.  .  .-. .-.   /-.  .-._.)  (    */
/*   By:             )/   )   )  /  /    (  |   )/   )   ) /   )(   )(    )   */
/*                  '/   /   (`.'  /      `-'-''/   /   (.'`--'`-`-'  `--':   */
/*   Created: 24-02-2022  by  `-'                        `-'                  */
/*   Updated: 24-02-2022 17:31 by                                             */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

#ifdef DO_BONUSES

# define SPEEDLINES_PATH "./speedlines_xpm_900-1600px/speedline_"

void	init_game(t_game *game, int width, int height)
{
	game->width = width;
	game->height = height;
	game->ceil_color = 0xFF000000;
	game->floor_color = 0xFF000000;
	game->player.pos.y = 1.5;
	game->player.pos.x = 1.5;
	game->player.direction.x = 1.0;
	game->player.direction.y = 0.0;
	game->player.cam_plane.x = 0.0;
	game->player.cam_plane.y = 1.0;
	game->player.velocity.x = 0.0;
	game->player.velocity.y = 0.0;
	game->player.velocity.z = 0.0;
	game->player.speed = 1.0;
	game->paused = 0;
	game->should_exit = 0;
	game->do_the_spin = 0;
	game->n_tex.image.img = NULL;
	game->e_tex.image.img = NULL;
	game->s_tex.image.img = NULL;
	game->w_tex.image.img = NULL;
}

void	*load_animated_sprites(void *arg)
{
	t_sprite	*sprite;
	char		*num;
	static char	file[60] = SPEEDLINES_PATH;
	int			i;
	int			z;

	sprite = (t_sprite *) arg;
	i = -1;
	while (++i <= 50)
	{
		num = ft_itoa(i);
		ft_strlcat(file, num, 60);
		free(num);
		ft_strlcat(file, ".xpm", 60);
		sprite->animation[i] = mlx_xpm_file_to_image(sprite->mlx, file, &z, &z);
		file[ft_strlen(SPEEDLINES_PATH)] = '\0';
		if (!sprite->animation[i])
		{
			while (--i >= 0)
				mlx_destroy_image(sprite->mlx, sprite->animation[i]);
			return ((void *) -1);
		}
	}
	return (NULL);
}

int	init_tex(void *mlx, t_texture *tex, char *filename)
{
	int	x_size;
	int	y_size;

	tex->image.img = mlx_xpm_file_to_image(mlx, filename, \
												&x_size, &y_size);
	if (!tex->image.img)
		return (-1);
	tex->height = y_size;
	tex->width = x_size;
	tex->image.addr = mlx_get_data_addr(tex->image.img, \
				&tex->image.bpp, &tex->image.line_length, \
				&tex->image.endian);
	if (!tex->image.addr)
	{
		mlx_destroy_image(mlx, tex->image.img);
		return (-1);
	}
	return (0);
}

int	init_beaux_gosses(t_game *game)
{
	if (init_tex(game->mlx, &(game->bg1), "./1.xpm") == -1)
		return (-1);
	if (init_tex(game->mlx, &(game->bg2), "./2.xpm") == -1)
	{
		mlx_destroy_image(game->mlx, game->bg1.image.img);
		return (-1);
	}
	return (0);
}
#endif