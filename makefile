# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
CC = g++
#CFLAGS  = -g -Wall -Weffc++
CFLAGS = -g -Wall -Weffc++ -std=c++11
LFLAGS  = -L/usr/lib

# All Targets
all: splflix

# Tool invocations
# Executable "splflix" depends on the files BaseAction.o, Session.o, User.o,Watchable.o and Main.o.
splflix: bin/User.o bin/BaseAction.o bin/Session.o bin/Watchable.o bin/Main.o
	@echo 'Building target: splflix'
	@echo 'Invoking: C++ Linker'
	$(CC) -o bin/splflix bin/User.o bin/BaseAction.o bin/Session.o bin/Watchable.o bin/Main.o $(LFLAGS)
	@echo 'Finished building target: splflix'
	@echo ' '

# Depends on the source and header files
bin/User.o: src/User.cpp
	$(CC) $(CFLAGS) -c -Linclude -o bin/User.o src/User.cpp	
	
# Depends on the source and header files
bin/BaseAction.o: src/BaseAction.cpp
	$(CC) $(CFLAGS) -c -Linclude -o bin/BaseAction.o src/BaseAction.cpp

# Depends on the source and header files
bin/Session.o: src/Session.cpp
	$(CC) $(CFLAGS) -c -Linclude -o bin/Session.o src/Session.cpp

# Depends on the source and header files
bin/Watchable.o: src/Watchable.cpp
	$(CC) $(CFLAGS) -c -Linclude -o bin/Watchable.o src/Watchable.cpp

# Depends on the source and header files 
bin/Main.o: src/Main.cpp
	$(CC) $(CFLAGS) -c -Linclude -o bin/Main.o src/Main.cpp

#Clean the build directory
clean: 
	rm -f bin/*
