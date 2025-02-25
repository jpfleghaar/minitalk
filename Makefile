# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jpflegha <jpflegha@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/19 15:29:36 by jpflegha          #+#    #+#              #
#    Updated: 2025/02/25 17:57:38 by jpflegha         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Compiler and flags
CC = cc
Cflags = -Wall -Wextra -Werror
RM = rm -f

# Source files
SRC_SERVER = server.c
SRC_CLIENT = client.c
SRC_UTILS = utils.c

# Object files
OBJ_SERVER = $(SRC_SERVER:.c=.o)
OBJ_CLIENT = $(SRC_CLIENT:.c=.o)
OBJ_UTILS = $(SRC_UTILS:.c=.o)

#name of the executable
SERVER = server
CLIENT = client

header = minitalk.h

# Default rule
all: $(SERVER) $(CLIENT)

# Compile the server
$(SERVER): $(OBJ_SERVER) $(OBJ_UTILS)
	$(CC) $(Cflags) -o $(SERVER) $(OBJ_SERVER) $(OBJ_UTILS)

# Compile the client
$(CLIENT): $(OBJ_CLIENT) $(OBJ_UTILS)
	$(CC) $(Cflags) -o $(CLIENT) $(OBJ_CLIENT) $(OBJ_UTILS)

#pattern rule for object files
%.o: %.c $(header)
	$(CC) $(Cflags) -c $< -o $@
	
# Clean the object files
clean:
	$(RM) $(OBJ_SERVER) $(OBJ_CLIENT) $(OBJ_UTILS)

# Clean everything
fclean: clean
	$(RM) $(SERVER) $(CLIENT)

# Recompile everything
re: fclean all

#tells Make that the targets are not files, so they always run and ignore existing files with the same name.
.PHONY: all clean fclean re