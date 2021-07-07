BIN := main
SOURCE_MAIN := main.cpp 
SOURCE_EXT := 
CC := g++
CPPFLAGS := -I./include -L./lib
LDFLAGS := -lsfml-graphics -lsfml-window -lsfml-system 

$(BIN):
	cls && $(CC) $(SOURCE_MAIN) $(SOURCE_EXT) $(CPPFLAGS) $(LDFLAGS) -o $(BIN).exe

clean:
	rm -rf $(BIN).exe