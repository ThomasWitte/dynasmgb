CC = gcc
CFLAGS = -std=gnu11 -g -Wall -Wextra -Wno-unused-parameter
LDFLAGS = -std=gnu11
LIBS = -lSDL2
BIN = dynasmgb
OBJ = core.o instructions.o lcd.o memory.o emit.o interrupt.o main.o memory_inspector.o

$(BIN): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $+ $(LIBS)
	
%.o: src/%.c
	$(CC) -c $(CFLAGS) $<
	
src/%.c: src/%.dasc
	lua dynasm/dynasm.lua -I src -o $@ $<
	
clean:
	rm -f $(BIN) $(OBJ) src/emit.c
