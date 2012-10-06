CXX=g++
CXXFLAGS=-g -O2 -Wall -ansi -Wno-deprecated
TARGET=douglas-peucker

BOOST_HOME = /Users/university/local/include
BOOST_INCLUDE = /Users/university/local/include
BOOST_LIB = /Users/university/local/lib

#BOOST_INCLUDE = /usr/local/boost_1_49_0
#BOOST_LIB = /usr/local/boost_1_49_0/stage/lib

INCLUDE= -I $(BOOST_INCLUDE)
LIBS= -L $(BOOST_LIB)

LDFLAGS = -lboost_system
LDFLAGS += -lboost_date_time
LDFLAGS += -lboost_filesystem
LDFLAGS += -lboost_graph

OBJS=main.o
OBJS+=Point.o
OBJS+=DouglasPeuckerReferenceAlgorithm.o
OBJS+=HaversineDistance.o

all:$(TARGET)
$(TARGET):$(OBJS)
	$(CXX) $(OBJS) $(LIBS) $(LDFLAGS) -o $(TARGET)

main.o:main.cpp Distance.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c main.cpp -o main.o

Point.o:Point.cpp Point.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c Point.cpp -o Point.o

DouglasPeuckerReferenceAlgorithm.o:DouglasPeuckerReferenceAlgorithm.cpp DouglasPeuckerReferenceAlgorithm.hpp Distance.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c DouglasPeuckerReferenceAlgorithm.cpp -o DouglasPeuckerReferenceAlgorithm.o

HaversineDistance.o:HaversineDistance.cpp HaversineDistance.hpp Distance.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c HaversineDistance.cpp -o HaversineDistance.o

EuclideanGeometry.o:EuclideanGeometry.cpp EuclideanGeometry.hpp Distance.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c EuclideanGeometry.cpp -o EuclideanGeometry.o

clean:
	rm $(OBJS) $(TARGET)
