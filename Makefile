# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: thepaqui <thepaqui@student.42nice.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/17 00:49:35 by thepaqui          #+#    #+#              #
#    Updated: 2023/12/21 03:33:05 by thepaqui         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME= bmp_view

CC= c++

CCFLAGS= -Wall -Wextra -Werror -std=c++2a

#LIBS= -lglfw -lGL -lm -lX11 -lpthread -lXi -lXrandr -ldl
LIBS= -lglfw -ldl

INCLUDES= -I./include/

SRCS=	src/main.cpp \
		src/glad.cpp \
		src/window.cpp \
		src/input.cpp \
		src/Texture.cpp \
		src/ShaderProgram.cpp \
		src/VAO.cpp \
		src/render.cpp

OBJS= $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(CCFLAGS) $(INCLUDES) $(OBJS) -o $(NAME) $(LIBS)

%.o : %.cpp
	$(CC) $(CCFLAGS) $(INCLUDES) -c $< -o $@ $(LIBS)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re