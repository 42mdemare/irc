# Variables

NAME = ircserv

CC = c++
CPPFLAGS += -Wall -Wextra -Werror -g -std=c++98

SRCS =	main.cpp \
		srcs/Server.cpp \
		srcs/CommandHandler.cpp \
		srcs/Client.cpp \
		srcs/Channel.cpp \
		srcs/commands/join.cpp \
		srcs/commands/nick.cpp \
		srcs/commands/pass.cpp \
		srcs/commands/user.cpp \
		srcs/commands/privmsg.cpp \
		srcs/commands/part.cpp \
		srcs/commands/pong.cpp \
		srcs/commands/kick.cpp \
		srcs/commands/invite.cpp \
		srcs/commands/topic.cpp \
		srcs/commands/mode.cpp \
		srcs/commands/who.cpp \
		srcs/config.cpp \
		srcs/asciiArt.cpp

OBJ_DIR = obj
OBJ = $(SRCS:%.cpp=$(OBJ_DIR)/%.o)

# Rules

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CPPFLAGS) $(OBJ) -o $(NAME)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
