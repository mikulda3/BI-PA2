# declare variables
CXX=g++
CFLAGS=-c -Wall -pedantic -Wextra -std=c++14
LIBS=-lncursesw
DIR=compiled

all: mikulda3

compile: ./mikulda3
	$(CXX) $(DIR)/Main.o $(DIR)/CApplication.o $(DIR)/CGame.o $(DIR)/CPlayer.o \
	$(DIR)/CGlobal.o $(DIR)/CGhost.o $(DIR)/CBlinky.o $(DIR)/CPinky.o \
	$(DIR)/CInky.o $(DIR)/CClyde.o -o ./mikulda3 $(LIBS)

./mikulda3: $(DIR)/Main.o $(DIR)/CApplication.o $(DIR)/CGame.o $(DIR)/CPlayer.o \
			$(DIR)/CGlobal.o $(DIR)/CGhost.o $(DIR)/CBlinky.o $(DIR)/CPinky.o \
			$(DIR)/CInky.o $(DIR)/CClyde.o

# compile each file
$(DIR)/Main.o: src/Main.cpp src/CApplication.hpp src/CGame.hpp src/CPlayer.hpp \
 		src/CGlobal.hpp src/CGhost.hpp src/CBlinky.hpp src/CInky.hpp \
 		src/CPinky.hpp src/CClyde.hpp
	mkdir -p $(DIR)
	$(CXX) $(CFLAGS) -o $(DIR)/Main.o src/Main.cpp $(LIBS)

$(DIR)/CApplication.o: src/CApplication.cpp src/CApplication.hpp src/CGame.hpp \
		src/CPlayer.hpp src/CGlobal.hpp src/CGhost.hpp src/CBlinky.hpp \
		src/CInky.hpp src/CPinky.hpp src/CClyde.hpp
	mkdir -p $(DIR)
	$(CXX) $(CFLAGS) -o $(DIR)/CApplication.o src/CApplication.cpp $(LIBS)

$(DIR)/CGame.o: src/CGame.cpp src/CGame.hpp src/CPlayer.hpp src/CGlobal.hpp \
 		src/CGhost.hpp src/CBlinky.hpp src/CInky.hpp src/CPinky.hpp \
 		src/CClyde.hpp
	mkdir -p $(DIR)
	$(CXX) $(CFLAGS) -o $(DIR)/CGame.o src/CGame.cpp $(LIBS)

$(DIR)/CPlayer.o: src/CPlayer.cpp src/CPlayer.hpp
	mkdir -p $(DIR)
	$(CXX) $(CFLAGS) -o $(DIR)/CPlayer.o src/CPlayer.cpp $(LIBS)

$(DIR)/CGlobal.o: src/CGlobal.cpp src/CGlobal.hpp
	mkdir -p $(DIR)
	$(CXX) $(CFLAGS) -o $(DIR)/CGlobal.o src/CGlobal.cpp $(LIBS)

$(DIR)/CGhost.o: src/CGhost.cpp src/CGhost.hpp src/CPlayer.hpp src/CGlobal.hpp
	mkdir -p $(DIR)
	$(CXX) $(CFLAGS) -o $(DIR)/CGhost.o src/CGhost.cpp $(LIBS)

$(DIR)/CBlinky.o: src/CBlinky.cpp src/CBlinky.hpp src/CGhost.hpp src/CPlayer.hpp
	mkdir -p $(DIR)
	$(CXX) $(CFLAGS) -o $(DIR)/CBlinky.o src/CBlinky.cpp $(LIBS)

$(DIR)/CInky.o: src/CInky.cpp src/CInky.hpp src/CGhost.hpp src/CPlayer.hpp \
 		src/CGlobal.hpp
	mkdir -p $(DIR)
	$(CXX) $(CFLAGS) -o $(DIR)/CInky.o src/CInky.cpp $(LIBS)

$(DIR)/CPinky.o: src/CPinky.cpp src/CPinky.hpp src/CGhost.hpp src/CPlayer.hpp
	mkdir -p $(DIR)
	$(CXX) $(CFLAGS) -o $(DIR)/CPinky.o src/CPinky.cpp $(LIBS)

$(DIR)/CClyde.o: src/CClyde.cpp src/CClyde.hpp src/CGhost.hpp src/CPlayer.hpp
	mkdir -p $(DIR)
	$(CXX) $(CFLAGS) -o $(DIR)/CClyde.o src/CClyde.cpp $(LIBS)

run: ./mikulda3
	./mikulda3

doc: Doxyfile src/*
	doxygen Doxyfile

clean:
	rm -r $(DIR)
	rm -r mikulda3
	rm -r doc



