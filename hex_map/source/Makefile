CXX = g++
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.c=.o)
TARGET = hex_map

all : $(TARGET)
	$(CXX) -z relro -z now -fPIE -pie -o $(TARGET) $(OBJS) -ldl
	strip -s $(TARGET)
	rm -f *.o

$(TARGET) :
	$(CXX) -c $(SRCS)

clean :
	rm -f $(TARGET)