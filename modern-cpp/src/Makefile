CXXFLAGS += -std=c++23 -MD -pipe -Wall -Wextra -Wpedantic -Wdeprecated-copy-dtor -fsanitize=address
SRCS = final.cpp main.cpp naive.cpp safe.cpp
OBJS = $(SRCS:.cpp=.o)
DEPS = $(SRCS:.cpp=.d)
PROG = modern

.PHONY: all clean

all: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(PROG)

clean:
	rm -f $(OBJS) $(DEPS) $(PROG)

-include $(DEPS)
