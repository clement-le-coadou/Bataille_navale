CC=gcc
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
LIB=libnaval.a

all : exec

%.o : %.c %.h
	$(CC) -c $< -o $@

$(LIB) : $(OBJ)
	ar rcs $@ $^
exec : $(LIB) main.o
	$(CC) -fsanitize=address -o $@ main.o -L. -lnaval

debug:
	@echo $(SRC)
	@echo $(OBJ)

clean : 
	rm -f *.o
	rm -f exec

doxy : 
	doxygen -g
	
