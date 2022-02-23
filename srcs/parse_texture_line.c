/* ************************************************************************** */
/*                                                                            */
/*                                  .-.                       .               */
/*                                 / -'                      /                */
/*                  .  .-. .-.   -/--).--..-.  .  .-. .-.   /-.  .-._.)  (    */
/*   By:             )/   )   )  /  /    (  |   )/   )   ) /   )(   )(    )   */
/*                  '/   /   (`.'  /      `-'-''/   /   (.'`--'`-`-'  `--':   */
/*   Created: 23-02-2022  by  `-'                        `-'                  */
/*   Updated: 23-02-2022 15:28 by                                             */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
			free(line);
		if (texture->image.img)
			return (print_error_plus_arg("Found a texture more than once.\n"));
		texture->image.img = mlx_xpm_file_to_image(game->mlx, args[1], \
								&texture->width, &texture->height);
		ft_free_str_array(args);
		if (!texture->image.img)
			free(line);
		if (!texture->image.img)
			return (print_error_plus_arg("Cannot convert XPM to image\n"));
	}
	return (0);
}
