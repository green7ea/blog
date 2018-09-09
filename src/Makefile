CFLAGS = -MD -pipe -Wall -O0 -std=c99 -g
CXXFLAGS = -MD -pipe -Wall -O0 -std=c++11 -g

PROG1 = simple
EX1 = add.o simple.o

OBJS = $(EX1) $(EX2)
DUMPS = $(OBJS:.o=.sym)
DEPS = $(OBJS:.o=.d)

.PHONY: all clean

all: $(EX1) $(DUMPS)
	$(CC) $(CFLAGS) $(EX1) -o $(PROG1)

clean:
	rm -f $(OBJS) $(PROG1) $(DUMPS) $(DEPS)

%.sym: %.o
	nm $< > $@

-include $(DEPS)
