# Compiler
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -Iinclude

# Directories
SRC_DIR = src/config
INC_DIR = include/config
OBJ_DIR = obj


SRCS = main.cpp \
       $(SRC_DIR)/ConfigLexer.cpp \
       $(SRC_DIR)/ConfigParser.cpp \
       $(SRC_DIR)/ConfigValidator.cpp \
       $(SRC_DIR)/HttpConfig.cpp \
       $(SRC_DIR)/LocationConfig.cpp \
       $(SRC_DIR)/ServerConfig.cpp \

OBJS = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRCS))

# Target executable
NAME = webserve

# Default target
all: $(NAME)

# Create object directory if it doesn't exist
$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link
$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

# Clean object files
clean:
	rm -rf $(OBJ_DIR)

# Clean object files + executable
fclean: clean
	rm -f $(NAME)

# Rebuild everything
re: fclean all

.PHONY: all clean fclean re