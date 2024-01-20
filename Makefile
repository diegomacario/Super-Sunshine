SRC_DIR := src
INC_DIR := inc
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
INC_FILES := $(wildcard $(INC_DIR)/*.h)
INCLUDE := -I${INC_DIR}

CC=gcc
CXX=g++ -std=c++11
RM=rm -f
CPPFLAGS=-g $(shell root-config --cflags)
LDFLAGS=-g $(shell root-config --ldflags)

OBJS=$(subst .cc,.o,$(SRC_FILES))

all: superSunshine

superSunshine: $(OBJS)
	$(CXX) $(LDFLAGS) $(INCLUDE) -o superSunshine $(OBJS)

depend: .depend

.depend: $(SRC_FILES) $(INC_FILES)
	$(RM) ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) superSunshine

distclean: clean
	$(RM) *~ .depend

include .depend

