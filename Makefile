##
## Makefile for Makefile in /home/nicolas/bin/Makefile
##
## Made by Nicolas Polomack
## Login   <nicolas.polomack@epitech.eu>
##
## Started on  Tue Nov 15 09:05:43 2016 Nicolas Polomack
## Last update Mon Sep 10 19:38:34 2017 nicolaspolomack
##

SRC	=	main.cpp		\
		init.cpp		\
		display.cpp		\
		shaders.cpp

OBJ	=	$(SRC:.cpp=.o)

CPPFLAGS=	-Iinclude -lsfml-graphics -lsfml-window -lsfml-system -lGLEW -lGL -lm -O2

REDDARK	=	\033[31;2m

RED	=	\033[31;1m

GREEN	=	\033[32;1m

YEL	=	\033[33;1m

BLUE	=	\033[34;1m

PINK	=	\033[35;1m

CYAN	=	\033[36;1m

RES	=	\033[0m

NAME	=	app

PROJ	=	SFML+OpenGL

UPPER	=	$(shell echo $(PROJ) | tr a-z A-Z)

all:	$(NAME)

$(NAME):$(OBJ)
	@echo
	@echo -e "$(GREEN)Everything compiled smoothly. Now compiling dependancies...$(RES)"
	@echo
	@echo -en "$(CYAN)Linking $(PROJ)...$(RES)"
	@g++ -o $(NAME) $(OBJ) $(CPPFLAGS)
	@echo -e "\t$(GREEN)OK$(RES)$(CYAN)!$(RES)"
	@echo
	@echo -e "$(GREEN)---- $(UPPER) READY ----$(RES)"
	@echo

clean:
	@$(foreach var, $(OBJ), if [ -e $(var) ] ; then \
	printf "[$(RED)RM$(RES)] $(YEL)$(var)$(RES)\n" \
	&& $(RM) $(var) ; fi ;)

fclean:	clean
	@if [ -e $(NAME) ] ; then \
	printf "[$(RED)RM EXEC$(RES)] $(YEL)$(NAME)$(RES)\n" \
	&& rm -f $(NAME) ; fi

re:	fclean all

%.o:	%.cpp
	@echo -e "[$(RED)COMPILE$(RES)] $(YEL)$<$(RES) $(BLUE)=>$(RES) $(YEL)$@$(RES)"
	@g++ $(CPPFLAGS) -o $@ -c $<
