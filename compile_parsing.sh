gcc ./srcs/parsing.c ./libft/libft.a \
	-lmlx -Iminilibx/ -Lminilibx/ -framework OpenGL -framework AppKit \
	-lz -Ofast -g -fsanitize=address \
	-o parsing
