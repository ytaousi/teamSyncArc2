
.PHONEY : all clean fclean re bonus run

## variables
NAME    = webserv
CC		= clang++
FLAGS   = -w -std=c++98 -D DEBUG -D CONSOLE_ON
#-Wall -Werror -Wextra

INCLUDES= includes 

UTILS	= helpers.cpp
CONFIG	= Configuration.cpp Parser.cpp Tokenizer.cpp Directive.cpp
REQ		= 
RES		= 
SERVER	= Cluster.cpp VServer.cpp Socket.cpp Request.cpp Response.cpp 

SRCS    = 	./src/webserv.cpp \
			$(UTILS:%.cpp=./src/utils/%.cpp)\
			$(CONFIG:%.cpp=./src/Configuration/%.cpp)\
			$(SERVER:%.cpp=./src/Networking/%.cpp)

HEADERS =	$(CONFIG:%.cpp=./src/Configuration/%.hpp)\
			$(SERVER:%.cpp=./src/Networking/%.hpp)\
			./src/utils/Console.hpp\
			./src/utils/Logger.hpp

## rules
$(NAME): $(SRCS) $(HEADERS)
	@$(CC) $(FLAGS) $(SRCS) -I $(INCLUDES) -o $(NAME)

all: $(NAME)

clean:
	@rm -rf *.o .objects/*.o

fclean: clean
	@rm -rf $(NAME)

re: fclean all

run: $(NAME)
	@./$(NAME) ./other/sample.conf

TRACE=network
strace: $(NAME)
	@strace -o .strace/$(shell date +"%d-%m-%y-[%H:%M:%S]").log -f -e trace=${TRACE} ./$(NAME) ./other/sample.conf