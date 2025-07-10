NAME = ircserv

SRCS = main.cpp srcs/Server.cpp srcs/CommandHandler.cpp srcs/Client.cpp srcs/Channel.cpp srcs/Bot.cpp \

OBJS = $(SRCS:.cpp=.o)

CC = c++
CPPFLAGS += -Wall -Wextra -Werror -g -std=c++98

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME)

%.o: %.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re