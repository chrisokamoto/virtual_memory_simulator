CC = gcc 
CFLAGS = -Wall -g -lm
BIN = tp3
RM = rm -f
SRC = $(wildcard src/*.c)
OBJS = $(SRC:.c=.o)
main = olex-ga
RUN_ARGS = input output
VALGRIND = valgrind --tool=memcheck --leak-check=yes --show-reachable=yes

all: $(main)

pagina: pagina.h
	$(CC) -c pagina.h $(CFLAGS)

fifo: fifo.c fifo.h
	$(CC) -c fifo.c fifo.h $(CFLAGS)

lfu: lfu.c lfu.h
	$(CC) -c lfu.c lfu.h $(CFLAGS)

lru: lru.c lru.h
	$(CC) -c lru.c lru.h $(CFLAGS)

$(main): $(OBJS)
	@echo " :: linkando"
	$(CC) -o $(BIN) main.c fifo.c pagina.h lru.c lfu.c $(CFLAGS)

%.o: src/%.c
	@echo " :: compilando \"$@\""
	$(CC) -c $(BIN) main.c  $(CFLAGS)

clean:
	$(RM) $(main) src/*.o
	clear

run: $(main)
	./$(BIN) $(RUN_ARGS) 

valgrind: $(main)
	  $(VALGRIND) ./$(BIN) $(RUN_ARGS) 


