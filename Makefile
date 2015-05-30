CC = gcc
CXX = g++
CFLAGS = -std=gnu11 -Wall -Wextra -Wno-unused-parameter `pkg-config --cflags glib-2.0`
CXXFLAGS = -std=c++14 -Wall -Wextra -Wno-unused-parameter -Wno-unused-value `pkg-config --cflags glib-2.0`
LDFLAGS = -std=gnu11
LIBS = -lSDL2 -lreadline `pkg-config --libs glib-2.0` -lm -lstdc++
BIN = dynasmgb
OBJ = core.o instructions.o lcd.o memory.o emit.o interrupt.o main.o \
      memory_inspector.o savestate.o sound_blargg.o optimize.tab.o optimize.o \
      Blip_Buffer.o Gb_Apu.o Gb_Oscs.o Multi_Buffer.o debugger.o

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

%.o: src/%.cpp src/optimize.tab.h
	$(CXX) -c $(CXXFLAGS) -I src/Gb_Snd_Emu-0.1.4/gb_apu $<
	
%.o: src/Gb_Snd_Emu-0.1.4/gb_apu/%.cpp
	$(CXX) -c $(CXXFLAGS) -I src/Gb_Snd_Emu-0.1.4/gb_apu $<
	
src/%.c: src/%.dasc
	lua5.2 dynasm/dynasm.lua $(DYNASMFLAGS) -I src -o $@ $<
	
src/%.tab.c: src/%.y
	yacc -d -o $@ $<

src/%.tab.h: src/%.tab.c
	@

clean:
	rm -f $(BIN) $(OBJ)
