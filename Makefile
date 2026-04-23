CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -I. -Iincludes/Config \
        -Iincludes/CGI -Iincludes/cookies -Iincludes/HTTP -Iincludes/Socket
NAME = webserv
OBJ_DIR = obj

GREEN = \033[1;32m
YELLOW = \033[1;33m
BLUE = \033[1;34m
RED = \033[1;31m
RESET = \033[0m

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
	@printf "$(BLUE)Compiling$(RESET) %s\n" $<
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	@printf "$(GREEN)Linking$(RESET) %s\n" $(NAME)
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
	@printf "$(GREEN)Done$(RESET) %s\n" $(NAME)

clean:
	@printf "$(YELLOW)Cleaning$(RESET) objects\n"
	@rm -rf $(OBJ_DIR)

fclean: clean
	@printf "$(YELLOW)Removing$(RESET) %s\n" $(NAME)
	@rm -f $(NAME)
       
re: fclean all

.PHONY: all clean fclean re
