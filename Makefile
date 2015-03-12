CC = gcc
CFLAGS = -std=gnu11 -g -Wall -Wextra -Wno-unused-parameter
LDFLAGS = -std=gnu11
LIBS = -lSDL2
BIN = dynasmgb
OBJ = core.o instructions.o lcd.o memory.o emit.o interrupt.o main.o memory_inspector.o sound.o

all: $(BIN)

debug: CFLAGS += -D DEBUG -D DEBUG_CG
debug: DYNASMFLAGS += -D DEBUG -D DEBUG_CG
debug: all

debug_cg: CFLAGS += -D DEBUG_CG
debug_cg: DYNASMFLAGS += -D DEBUG_CG
debug_cg: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $+ $(LIBS)
	
%.o: src/%.c
	$(CC) -c $(CFLAGS) $<
	
src/%.c: src/%.dasc
	lua dynasm/dynasm.lua $(DYNASMFLAGS) -I src -o $@ $<
	
clean:
	rm -f $(BIN) $(OBJ)
