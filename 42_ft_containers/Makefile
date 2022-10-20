CXX= c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 #-g3 -fsanitize=address
SRCS = main.cpp
OBJS = ${SRCS:.cpp=.o}
RM = rm -f
NAME = a.out

all : $(NAME)

$(NAME) : ${OBJS}
		${CXX} ${CXXFLAGS} ${OBJS} -o a.out

%.o : %.cpp
		${CXX} ${CXXFLAGS} -c $< -o ${<:.cpp=.o}

clean :
		${RM} ${OBJS}

fclean : clean
		${RM} $(NAME)
re : 
	${MAKE} fclean 
	${MAKE} all

.PHONY: all clean fclean re