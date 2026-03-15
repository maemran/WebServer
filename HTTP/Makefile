CXX = c++
CXXFLAGS =  -Wall -Wextra -Werror -std=c++98 -g
SRC = main.cpp HttpRequest.cpp URI.cpp ../HttpConfig.cpp ../LocationConfig.cpp\
 ../ServerConfig.cpp HttpResponse.cpp router.cpp
OBJ_DIR = OBJ
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.cpp=.o))
NAME = a.out

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

$(OBJ_DIR)/%.o:%.cpp  HttpRequest.hpp URI.hpp | $(OBJ_DIR) 
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR) 

clean:
	rm -fr $(OBJ_DIR)

fclean: clean
	rm -fr $(NAME)

re: fclean all

.PHONY: all clean fclean re