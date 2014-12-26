LIB = ../lib

all:client server

client:client.cpp
	g++ -o client client.cpp -I $(LIB)

server:server.cpp
	g++ -o server server.cpp -I $(LIB)

clean:
	rm client server
