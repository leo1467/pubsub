SRCFILES_SERVER=common.cc $(wildcard server*.c) $(wildcard server*.cc) $(wildcard server*.cpp)
SRCFILES_CLIENT=common.cc $(wildcard client*.c) $(wildcard client*.cc) $(wildcard client*.cpp)

OBJS_SERVER=$(patsubst %.c, %.o, $(patsubst %.cc, %.o, $(patsubst %.cpp, %.o, $(SRCFILES_SERVER))))
OBJS_CLIENT=$(patsubst %.c, %.o, $(patsubst %.cc, %.o, $(patsubst %.cpp, %.o, $(SRCFILES_CLIENT))))

all:clean server client

server:$(OBJS_SERVER)
	g++ -I. -Wall -Wextra -g -O0 -std=c++17 $^ -o $@

client:$(OBJS_CLIENT)
	g++ -I. -Wall -Wextra -g -O0 -std=c++17 $^ -o $@

%.o:%.c
	g++ -Wall -Wextra -g -O0 -std=c++17 -c $< -o $@

%.o:%.cc
	g++ -Wall -Wextra -g -O0 -std=c++17 -c $< -o $@

%.o:%.cpp
	g++ -Wall -Wextra -g -O0 -std=c++17 -c $< -o $@

clean:
	rm -f *.o server client
