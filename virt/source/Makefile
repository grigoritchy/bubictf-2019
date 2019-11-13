CXX = g++

SERVER_SRCS = $(wildcard *.cpp)
SERVER_OBJS = $(SERVER_SRCS:.c=.o)
SERVER_TARGET = virt

all : $(SERVER_TARGET)
	# $(CXX) -o $(CLINET_TARGET) $(CLIENT_OBJS) -fpermissive
	# strip -s $(CLINET_TARGET)
	# rm -f client/*.o

	$(CXX) -o $(SERVER_TARGET) $(SERVER_OBJS)
	strip -s $(SERVER_TARGET)
	
	rm -f *.o

$(SERVER_TARGET) :
	$(CXX) -c $(SERVER_SRCS)

clean :
	rm -f $(SERVER_TARGET)
