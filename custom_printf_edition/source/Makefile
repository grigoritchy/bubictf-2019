CXX = gcc
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
TARGET = custom_printf_edition

all : $(TARGET)
	$(CXX) -o $(TARGET) $(OBJS) -ldl
	strip -s $(TARGET)
	rm -f *.o

$(TARGET) :
	$(CXX) -c $(SRCS)

clean :
	rm -f $(TARGET)