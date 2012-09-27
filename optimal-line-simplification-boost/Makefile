CXX = g++

# see http://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
# -g generate debuging symbols
# -O2 optimization level
# -Wall warnings for all syntactically correct but semantically suspicious
# -ansi tell compiler to enforce ANSI C standards
# -Wno-deprecated Do not warn about usage of deprecated features
# -arch x86_64 64bit architecture
CXXFLAGS = -g -O2 -Wall -ansi -Wno-deprecated
LDFLAGS =
INCLUDES=
LIBS =
OBJS = src/linear-approximation.o
TARGET = la

.PHONY: clean

all: $(OBJS)
	$(CXX) $(LIBS) $(OBJS) $(LDFLAGS) -o $(TARGET)
	
linear-approximation.o: src/linear-approximation.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c src/linear-approximation.cpp  -o linear-approximation.o

chmod:
	chmod u+x $(TARGET)
clean:
	rm -f $(TARGET)
	rm -f $(OBJS)