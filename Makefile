CC = gcc
CFLAGS = -std=gnu11 -Wall -Wextra -Wno-unused-parameter `pkg-config --cflags glib-2.0`
LDFLAGS = -std=gnu11
LIBS = -lSDL2 `pkg-config --libs glib-2.0`
BIN = dynasmgb
OBJ = core.o instructions.o lcd.o memory.o emit.o interrupt.o main.o memory_inspector.o sound.o savestate.o optimize.tab.o

all: CFLAGS += -O3 -flto
all: LDFLAGS += -O3 -flto
all: $(BIN)

debug: CFLAGS += -g -D DEBUG -D DEBUG_CG
debug: DYNASMFLAGS += -D DEBUG -D DEBUG_CG
debug: $(BIN)

debug_cg: CFLAGS += -D DEBUG_CG
debug_cg: DYNASMFLAGS += -D DEBUG_CG
debug_cg: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $+ $(LIBS)
	
%.o: src/%.c src/optimize.tab.h
	$(CC) -c $(CFLAGS) $<
	
src/%.c: src/%.dasc
	lua dynasm/dynasm.lua $(DYNASMFLAGS) -I src -o $@ $<
	
src/%.tab.c: src/%.y
	yacc -d -o $@ $<

src/%.tab.h: src/%.tab.c
	@

clean:
	rm -f $(BIN) $(OBJ)
