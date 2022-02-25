SRCS =	check_map.c				check_map_algorithm.c	coordinates_utils.c	dda_algo.c \
		dda_algo2.c				dda_utils.c				doors_bonus.c		doors_lst_bonus.c \
		frame_counter_bonus.c	free_all.c				ft_utils.c			hitboxes_bonus.c \
		init_all.c				init_all2_bonus.c		↑↑↓↓←→←→AB_bonus.c	key_press_handling.c \
		key_release_handling.c	main.c					main_bonus.c		minimap_bonus.c \
		mouse_handling_bonus.c	movements.c				parse_color_line.c	parse_header.c \
		parse_map.c				parse_map2.c			mlx_utils.c			parse_player.c \
		parse_texture_line.c	parsing.c				parsing_utils.c		parsing_utils2.c \
		point_utils.c			rendering.c				rendering_2.c		triangles.c \
		vectors.c					

SRCS_BONUS =	check_map_algorithm_bonus.c check_map_bonus.c coordinates_utils_bonus.c dda_algo2_bonus.c \
				dda_algo_bonus.c dda_utils_bonus.c doors_bonus.c doors_lst_bonus.c frame_counter_bonus.c \
				free_all_bonus.c ft_utils_bonus.c hitboxes_bonus.c init_all2_bonus.c init_all_bonus.c \
				key_press_handling_bonus.c key_release_handling_bonus.c main_bonus.c minimap_bonus.c \
				mlx_utils_bonus.c mouse_handling_bonus.c movements_bonus.c parse_color_line_bonus.c \
				parse_header_bonus.c parse_map2_bonus.c parse_map_bonus.c parse_player_bonus.c \
				parse_texture_line_bonus.c parsing_bonus.c parsing_utils2_bonus.c parsing_utils_bonus.c \
				point_utils_bonus.c rendering_2_bonus.c rendering_bonus.c triangles_bonus.c vectors_bonus.c \
				↑↑↓↓←→←→AB_bonus.c init_all3_bonus.c

OBJS = $(addprefix ./srcs/, $(SRCS:.c=.o))
OBJS_BONUS = $(addprefix ./srcs_bonus/, $(SRCS_BONUS:.c=.o))

NAME = cub3d
NAME_BONUS = cub3d_bonus
LIBFT = ./libft/libft.a
MLX = -lmlx -Iminilibx/ -Lminilibx/ -framework OpenGL -framework AppKit -lz
CC = gcc -Wall -Wextra -Werror -O3 -g -fsanitize=address

SHELL = zsh

AQUA = \033[0;96m
AQUA_BOLD = \033[1;96m

PURPLE = \033[0;95m
PURPLE_BOLD = \033[1;95m

GREEN = \033[0;92m
GREEN_BOLD = \033[1;92m
GREEN_UNDRLINE = \033[4;32m

RED = \033[0;91m
IRED = \033[0;31m
RED_BOLD = \033[1;91m

SAME_LINE = \033[0G\033[2K

RESET = \033[0m

ifndef BONUS
%.o: %.c
	@$(CC) -c $< -o $@
	@echo -n "$(SAME_LINE)$(AQUA)Compiling $(AQUA_BOLD)$<$(RESET)"

else
%.o: %.c
	@$(CC) -DDO_BONUSES=1 -c $< -o $@
	@echo -n "$(SAME_LINE)$(AQUA)Compiling $(AQUA_BOLD)$<$(RESET)"
endif

$(NAME):	$(LIBFT) $(OBJS) 
	@echo
	@echo "$(PURPLE)Linking $(PURPLE)*.o into $(PURPLE_BOLD)$(NAME)$(RESET)"
	@$(CC) $(OBJS) $(MLX) $(LIBFT) -o $(NAME)
	@echo "$(GREEN_BOLD)Done compiling $(GREEN_UNDERLINE)$(NAME)"

$(NAME_BONUS):	$(LIBFT) $(OBJS_BONUS)
	@echo
	@echo "$(PURPLE)Linking $(PURPLE)*.o into $(PURPLE_BOLD)$(NAME_BONUS)$(RESET)"
	@$(CC) -DDO_BONUSES=1 $(OBJS_BONUS) $(MLX) $(LIBFT) -o $(NAME_BONUS)
	@echo "$(GREEN_BOLD)Done compiling $(GREEN_UNDERLINE)$(NAME_BONUS)"

bonus:
	@$(MAKE) $(NAME_BONUS) BONUS=1

$(LIBFT):
	@$(MAKE) -C ./libft NO_FT_PRINTF=1

all:		$(NAME)

clean:
	@rm -f $(OBJS)
	@rm -f $(OBJS_BONUS)
	@echo "$(RED)Removing $(IRED)*.o$(RESET)"
	@make -C ./libft clean

fclean:		clean
	@rm -f $(NAME)
	@rm -f $(NAME_BONUS)
	@echo "$(RED)Removing $(IRED)$(NAME)$(RESET)"
	@echo "$(RED)Removing $(IRED)$(NAME_BONUS)$(RESET)"
	@make -C ./libft fclean

re:			fclean all

.PHONY:	all clean fclean re