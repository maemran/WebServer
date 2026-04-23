CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -I. -Iincludes/Config \
        -Iincludes/CGI -Iincludes/cookies -Iincludes/HTTP -Iincludes/Socket
NAME = webserv
OBJ_DIR = obj

SRCS = \
       srcs/main.cpp \
       srcs/Config/ConfigLexer.cpp \
       srcs/Config/ConfigParser.cpp \
       srcs/Config/ConfigValidator.cpp \
       srcs/Config/HttpConfig.cpp \
       srcs/Config/LocationConfig.cpp \
       srcs/Config/ServerConfig.cpp \
       srcs/HTTP/HttpRequest.cpp \
       srcs/HTTP/HttpResponse.cpp \
       srcs/HTTP/router.cpp \
       srcs/HTTP/URI.cpp \
       srcs/CGI/CgiHandler.cpp \
       srcs/cookies/Cookie.cpp \
       srcs/cookies/Session.cpp \
       srcs/Socket/connection.cpp \
       srcs/Socket/serversocket.cpp

OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))

all: $(NAME)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)
       
re: fclean all

.PHONY: all clean fclean re
