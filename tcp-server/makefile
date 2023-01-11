servers:servers.o
	g++ -std=c++11 servers.o -o servers  -lpthread

servers.o:servers.cpp
	g++ -std=c++11 -c servers.cpp 

clean:
	rm -rf *.o servers

