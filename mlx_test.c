#include "minilibx/mlx.h"
#include <stdlib.h>
#include "cub3d.h"
#include <stdio.h>
#include <sys/time.h>

t_game game;

void print_elapsed(char str[], struct timeval start)
{
	struct timeval now;

	gettimeofday(&now, NULL);
	int diff_s = now.tv_sec - start.tv_sec;
	int diff_u = now.tv_usec - start.tv_usec;
	printf("%s %d ms elapsed\n", str, (diff_s * 1000 + diff_u / 1000));
}

int main()
{
	int width = 1280;
	int height = 720;

	game.mlx = mlx_init();
	game.window = mlx_new_window(game.mlx, width, height, "Cub3D");
	game.main_img.img = mlx_new_image(game.mlx, width, height);
	game.main_img.addr = mlx_get_data_addr(game.main_img.img, &game.main_img.bits_per_pixel, &game.main_img.line_length, &game.main_img.endian);

	int png_width;
	int png_height;

	/*
	struct timeval start;
	gettimeofday(&start, NULL);
	print_elapsed("start:", start);
	*/

	void *img_test = mlx_png_file_to_image(game.mlx, "test_png.png", &png_width, &png_height);

	//print_elapsed("png to image:", start);

	if (img_test)
		mlx_put_image_to_window(game.mlx, game.window, img_test, 0, 0);
	
	//print_elapsed("iamge to window:", start);
	mlx_loop(game.mlx);
}