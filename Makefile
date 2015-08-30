#    This file is part of dynasmgb.
#
#    dynasmgb is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    dynasmgb is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with dynasmgb.  If not, see <http://www.gnu.org/licenses/>.
#    
#    Copyright 2014, 2015 Thomas Witte

CC = gcc
CXX = g++
CFLAGS = -std=gnu11 -Wall -Wextra -Wno-unused-parameter `pkg-config --cflags glib-2.0`
CXXFLAGS = -std=c++14 -Wall -Wextra -Wno-unused-parameter -Wno-unused-value `pkg-config --cflags glib-2.0`
LDFLAGS = -std=gnu11
LIBS = -lSDL2 -lreadline `pkg-config --libs glib-2.0` -lm -lstdc++
BIN = dynasmgb
OBJ = core.o instructions.o lcd.o memory.o emit.o interrupt.o main.o \
      memory_inspector.o savestate.o sound_blargg.o optimize.o \
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
	
%.o: src/%.c
	$(CC) -c $(CFLAGS) $<

%.o: src/%.cpp
	$(CXX) -c $(CXXFLAGS) -I Gb_Snd_Emu-0.1.4/gb_apu $<
	
%.o: Gb_Snd_Emu-0.1.4/gb_apu/%.cpp
	$(CXX) -c $(CXXFLAGS) -I Gb_Snd_Emu-0.1.4/gb_apu $<
	
src/%.c: src/%.dasc
	lua5.2 dynasm/dynasm.lua $(DYNASMFLAGS) -I src -o $@ $<
	
clean:
	rm -f $(BIN) $(OBJ)
