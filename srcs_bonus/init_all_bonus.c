/* ************************************************************************** */
/*                                                                            */
/*                                  .-.                       .               */
/*                                 / -'                      /                */
/*                  .  .-. .-.   -/--).--..-.  .  .-. .-.   /-.  .-._.)  (    */
/*   By:             )/   )   )  /  /    (  |   )/   )   ) /   )(   )(    )   */
/*                  '/   /   (`.'  /      `-'-''/   /   (.'`--'`-`-'  `--':   */
/*   Created: 24-02-2022  by  `-'                        `-'                  */
/*   Updated: 25-02-2022 13:51 by                                             */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

#ifdef DO_BONUSES

# include <pthread.h>
# define SPEEDLINES_PATH "./textures/speedlines/speedline_"

int		init_beaux_gosses(t_game *game);
int		init_tex(void *mlx, t_texture *tex, char *filename);
void	*load_animated_sprites(void *arg);
void	init_game(t_game *game, int width, int height);

/*
	Initialize pause screen replacing transparent pixels with semi transparent
*/
static int	init_pause_screen(t_game *game)
{
	int		pouet;
	t_point	px;

	game->pause_screen.img = mlx_xpm_file_to_image(game->mlx, \
							"./textures/pause_menu.xpm", &pouet, &pouet);
	if (!game->pause_screen.img)
		return (-1);
	game->pause_screen.addr = mlx_get_data_addr(game->pause_screen.img, \
				&game->pause_screen.bpp, &game->pause_screen.line_length, \
				&game->pause_screen.endian);
	if (!game->pause_screen.addr)
		return (-1);
	px.x = -1;
	while (++(px.x) < 1600)
	{
		px.y = -1;
		while (++(px.y) < 900)
			if (((unsigned int *) game->pause_screen.addr) \
						[px.y * 1600 + px.x] == 0xFF000000)
				((unsigned int *) game->pause_screen.addr) \
						[px.y * 1600 + px.x] = 0x88888888;
	}
	return (0);
}

void	init_animation(pthread_t *init_img_thread, t_sprite *animation, \
						void *mlx)
{
	int			i;

	i = 0;
	while (i <= 50)
		animation->animation[i++] = NULL;
	animation->mlx = mlx;
	pthread_create(init_img_thread, NULL, &load_animated_sprites, animation);
}

int	init_door_tex(t_game *game)
{
	if (init_tex(game->mlx, &(game->door), DOOR_TEXTURE) == -1)
		return (-1);
	return (0);
}

int	init_all(t_game *game, pthread_t *anim_thread, char *cub_file)
{
	if (init_beaux_gosses(game) == -1)
		return (-1);
	if (init_pause_screen(game) == -1)
		return (-1);
	if (init_door_tex(game) == -1)
		return (-1);
	init_animation(anim_thread, &(game->animation), game->mlx);
	if (parse_cub_file(cub_file, game) == -1)
		return (-1);
	if (init_mlx_images_and_textures(game))
		return (-1);
	return (0);
}

#endif