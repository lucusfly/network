ifeq ($(dd),debug) 
	export dd := debug
	CPPLAGS=-g -W 
else  #release
	export dd := release
	CPPLAGS=-W
endif

LIB_PATH = ../../lib
UTIL_PATH = ../util
LIB_SEARCH_COMM=$(LIB_HOME)/search_module/common

SRCS=$(wildcard ./*.cpp $(LIB_PATH)/*.cpp $(UTIL_PATH)/*.cpp)
OBJS=$(patsubst %.cpp,%.o,$(SRCS))
TARGET=server

INC_PATH=-I $(LIB_PATH) -I $(UTIL_PATH)

.SECONDARY :%.o %(OBJS)

.cpp.o:
	@echo "Compile $@ ($<)."
	@g++ $(CPPLAGS) $(INC_PATH) -c $< -o $@

.c.o:
	@echo "Compile $@ ($<)."
	@gcc $(CPPLAGS) $(INC_PATH) -c $< -o $@

all: $(TARGET)

$(TARGET) : $(OBJS)
	@echo 'build $(TARGET)'
	g++ $(CPPLAGS) -o $(TARGET) $(OBJS) $(INC_PATH)

in:
	mv server ../runningspace

clean:
	@echo 'clean up the generated files'
	rm -f $(OBJS) $(TARGET)

.PHONY: all so test install clean
