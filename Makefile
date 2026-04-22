CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -I. -IConfig/include -IHTTP/include
NAME = webserve
OBJ_DIR = obj

SRCS = \
       main.cpp \
       Config/srcs/ConfigLexer.cpp \
       Config/srcs/ConfigParser.cpp \
       Config/srcs/ConfigValidator.cpp \
       Config/srcs/HttpConfig.cpp \
       Config/srcs/LocationConfig.cpp \
       Config/srcs/ServerConfig.cpp \
       HTTP/srcs/HttpRequest.cpp \
       HTTP/srcs/HttpResponse.cpp \
       HTTP/srcs/router.cpp \
       HTTP/srcs/URI.cpp \
       cgi/CgiHandler.cpp \
       cookies/Cookie.cpp \
       cookies/Session.cpp \
       connection.cpp 
       serversocket.cpp

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
