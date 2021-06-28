#	Compiler
CC	=	gcc
#	Compiler Flags
CFLAGS	=	-O3 -Wall -std=c99
#	Compiler Libraries
LIBS=	-lm
#	name of the Program
EXECUTABLE	=	campista
#	Sources
SRC	=	$(wildcard	*.c)
#	Headers
HDRS	=	$(wildcard	*.h)
#	Objects
OBJ	=	$(SRC:.c=.o)
#	cria os fichiros *.o
%.o: %.c HDRS
	$(CC) $(CFLAGS) -c $<

#TODO
#não sei oq isto é
dirs = $(shell ls testfiles)

#gera o executável
$(EXECUTABLE):	$(OBJ)
	$(CC)	-o $(EXECUTABLE)	$(OBJ)	$(LIBS)

v:
	valgrind --leak-check=full ./$(EXECUTABLE) testfilesAeB/${file *.camp}
	valgrind --leak-check=full --track-origins=yes -v --show-leak-kinds=all ./$(EXECUTABLE) boring05.camp


O3:
	$(CC) ${SRC} -O3 -Wall -o campista $(LIBS)
	clear

clean::
	rm	-f	*.o	core	campista  *.tent0~
	clear
#rm	-f	src/*.o	core	tuktuk  testfiles/*.walks	*~
