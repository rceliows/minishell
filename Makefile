# Standard
NAME			= minishell

# Directories
LIBFT_DIR		= ./libft
LIBFT			= $(LIBFT_DIR)/libft.a
INC				= inc/
SRC_DIR			= src/
OBJ_DIR			= obj/

# Compiler and Flags
CC				= cc
CFLAGS			= -Wall -Wextra -Werror -I$(INC) -I$(LIBFT_DIR)/inc
LDFLAGS			= -lreadline
RM				= rm -f
AR				= ar rcs

# Source Files

BUILTIN			= $(SRC_DIR)builtin_commands/builtin_commands.c \
				$(SRC_DIR)builtin_commands/builtin_commands_utils.c \
				$(SRC_DIR)builtin_commands/cd.c \
				$(SRC_DIR)builtin_commands/echo.c \
				$(SRC_DIR)builtin_commands/env.c \
				$(SRC_DIR)builtin_commands/exit.c \
				$(SRC_DIR)builtin_commands/export.c \
				$(SRC_DIR)builtin_commands/export_utils.c \
				$(SRC_DIR)builtin_commands/pwd.c \
				$(SRC_DIR)builtin_commands/unset.c \
		
ENV				= $(SRC_DIR)environment/envp_conversion.c \
				$(SRC_DIR)environment/envp_conversion_utils.c \
				$(SRC_DIR)environment/envp_expansion.c \
				$(SRC_DIR)environment/envp_expansion_utils.c \
				$(SRC_DIR)environment/envp_expansion_utils_2.c \
				$(SRC_DIR)environment/envp_management.c \
				$(SRC_DIR)environment/hash_table.c \
				$(SRC_DIR)environment/hash_table_utils.c \
				$(SRC_DIR)environment/hash_table_utils_2.c \

MAIN			= $(SRC_DIR)main/main.c \
				$(SRC_DIR)main/prompt.c \
				$(SRC_DIR)main/shell_loop.c \

LEXER			= $(SRC_DIR)lexer/lexer.c \
				$(SRC_DIR)lexer/lexer_expansion.c \
				$(SRC_DIR)lexer/lexer_handlers.c \
				$(SRC_DIR)lexer/lexer_utils.c \

PARSER	 		= $(SRC_DIR)parser/parser.c \
				$(SRC_DIR)parser/parser_utils.c \
				$(SRC_DIR)parser/parser_utils_2.c \
				$(SRC_DIR)parser/path_helpers.c \
				$(SRC_DIR)parser/tokens_utils.c \

EXECUTOR	 	= $(SRC_DIR)executor/executor.c \
				$(SRC_DIR)executor/executor_builtins.c \
				$(SRC_DIR)executor/executor_helpers.c \
				$(SRC_DIR)executor/executor_helpers1.c \
				$(SRC_DIR)executor/executor_helpers2.c \
				$(SRC_DIR)executor/executor_utils.c \

HEREDOC 		= $(SRC_DIR)heredoc/heredoc.c \
				$(SRC_DIR)heredoc/heredoc_child.c \
				$(SRC_DIR)heredoc/heredoc_child_expanded.c \
				$(SRC_DIR)heredoc/heredoc_utils.c \

SIGNALS			= $(SRC_DIR)signals/signals.c \
				$(SRC_DIR)signals/signals_utils.c \

UTILS			= $(SRC_DIR)utils/error_handling.c \
				$(SRC_DIR)utils/memory_utils.c \
				$(SRC_DIR)utils/string_utils.c \
				$(SRC_DIR)utils/free.c \

# Default to mandatory sources
SRCS			= $(BUILTIN) $(COMMANDS) $(ENV) $(UTILS) $(MAIN) $(PIPES) $(LEXER) $(PARSER) $(EXECUTOR) $(HEREDOC) $(SIGNALS) 

# Apply the pattern substitution
OBJ				= $(patsubst $(SRC_DIR)%.c,$(OBJ_DIR)%.o,$(SRCS))

# Build rules
all:			$(NAME)

$(NAME):		$(LIBFT) $(OBJ)
			@echo "Linking $(NAME)..."
			@$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(LDFLAGS) -o $(NAME)
			@echo "Build complete!"

# Check if libft directory exists, if not clone it
$(LIBFT_DIR):
			@echo "libft not found. Cloning from repository..."
			@git clone git@github.com:rceliows/libft.git $(LIBFT_DIR)
			@echo "libft cloned successfully!"

$(LIBFT):		$(LIBFT_DIR)
			@echo "Building libft..."
			@make -C $(LIBFT_DIR)
			@echo "libft built successfully!"

# Compile object files from source files
$(OBJ_DIR)%.o:		$(SRC_DIR)%.c 
			@mkdir -p $(@D)
			@$(CC) $(CFLAGS) -c $< -o $@

clean:
			@echo "Cleaning object files..."
			@$(RM) -r $(OBJ_DIR)
			@if [ -d "$(LIBFT_DIR)" ]; then make clean -C $(LIBFT_DIR); fi
			@echo "Clean complete!"

fclean:			clean
			@echo "Removing executables..."
			@$(RM) $(NAME)
			@if [ -d "$(LIBFT_DIR)" ]; then $(RM) $(LIBFT); fi
			@echo "Full clean complete!"

fcleanall:		fclean
			@echo "Removing library directories..."
			@if [ -d "$(LIBFT_DIR)" ]; then rm -rf $(LIBFT_DIR); echo "Removed $(LIBFT_DIR)"; fi
			@echo "Full clean with libraries complete!"

re:			fclean all

reall:			fcleanall all

# Phony targets represent actions not files
.PHONY:			all clean fclean fcleanall re reall
