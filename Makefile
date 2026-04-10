CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -I./HTTP

SRC = serversocket.cpp HttpConfig.cpp ServerConfig.cpp LocationConfig.cpp connection.cpp \
	HTTP/main.cpp HTTP/HttpRequest.cpp HTTP/HttpResponse.cpp HTTP/router.cpp HTTP/URI.cpp

NAME = socket

OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)
	
clean:
	rm -rf $(OBJ)
	rm -rf HTTP/*.o

fclean: clean
	rm -rf $(NAME)

re : fclean all

.PHONY: all clean fclean re
