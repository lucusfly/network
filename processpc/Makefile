LIB = ../lib

all:client server testcli

client:client.cpp
	g++ -o client client.cpp -I $(LIB)

server:server.cpp
	g++ -o server server.cpp -I $(LIB)

testcli:testcli.cpp
	g++ -o testcli testcli.cpp -I $(LIB)

clean:
	rm client server testcli
