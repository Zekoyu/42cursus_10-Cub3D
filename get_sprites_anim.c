#include "cub3d.h"
#include "libft/libft.h"
#include "minilibx/mlx.h"

/*
	Frees the itoa
*/
char	*ft_strcatdup(char *s1, char *s2)
{
	char	*res;
	int		res_len;

	res_len = (ft_strlen(s1) + ft_strlen(s2) + ft_strlen(".png") + 1);
	res = malloc(sizeof(char) * res_len);
	res[0] = '\0';
	ft_strlcat(res, s1, res_len);
	ft_strlcat(res, s2, res_len);
	res[res_len - 1] = '\0';
	res[res_len - 2] = 'g';
	res[res_len - 3] = 'n';
	res[res_len - 4] = 'p';
	res[res_len - 5] = '.';
	free(s2);
	return (res);
}

t_img	*get_animated_sprite_array(void *mlx, char *img_basename, int img_count)
{
	t_img	*res;
	t_img	curr_img;
	char	*current_file;
	char	*basename;
	int		res_index;
	int		x;
	int		y;

	res = malloc(sizeof(t_img) * (img_count));
	res_index = 0;
	basename = ft_strdup(img_basename);
	while (res_index < img_count)
	{
		current_file = ft_strcatdup(basename, ft_itoa(res_index));
		res[res_index].img = mlx_png_file_to_image(mlx, current_file, &x, &y);;
		res[res_index].width = x;
		res[res_index++].height = y;
		free(current_file);
	}
	free (basename);
	return (res);
}