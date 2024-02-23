#!/bin/sh

# Clean up any previous build artifacts in the libft directory
make -C libft/ fclean

# Build the libft library
make -C libft/

# Compile get_next_line.c with warnings as errors, debugging information, and the
# libft headers, creating an object file
clang -Wall -Wextra -Werror -g -I libft/includes -o get_next_line.o -c get_next_line.c

# Compile main.c with warnings as errors, debugging information, and the libft
# headers, creating an object file
clang -Wall -Wextra -Werror -g -I libft/includes -o main.o -c main.c

# Link the object files, libft library, and libft headers into an executable
clang -o test_gnl main.o get_next_line.o -I libft/includes -L libft/ -lft

