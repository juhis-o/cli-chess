CC = g++
CFLAGS = -std=c++20 -Wall
LIBS = -lncurses

SRCS = main.cpp chess.cpp kirous.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = a.out

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -g $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)
