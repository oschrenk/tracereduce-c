CXX=g++
CXXFLAGS=-g -Wall
TARGET=ref
BOOST_HOME = /usr/local/boost_1_49_0
BOOST_INCLUDE =  /usr/local/boost_1_49_0
BOOST_LIB = /usr/local/boost_1_49_0/stage/lib

INCLUDE= -I $(BOOST_INCLUDE)
LIBS= -L $(BOOST_LIB)

LDFLAGS = -lboost_date_time
LDFLAGS += -lboost_filesystem
LDFLAGS += -lboost_graph

OBJS=main.o
OBJS+=Point.o
OBJS+=LinearApproximationReferenceAlgorithm.o
OBJS+=EuclideanGeometry.o

all:$(TARGET)
$(TARGET):$(OBJS)
	$(CXX) $(OBJS) $(LIBS) $(LDFLAGS) -o $(TARGET)

main.o:main.cpp LinearApproximationReferenceAlgorithm.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c main.cpp -o main.o
	
EuclideanGeometry.o:EuclideanGeometry.cpp EuclideanGeometry.hpp Geometry.hpp
	$(CXX) $(CXXFLAGS) -c EuclideanGeometry.cpp -o EuclideanGeometry.o
	
Vertex.o: Vertex.hpp
	$(CXX) $(CXXFLAGS) -c Vertex.cpp -o Vertex.o

DijkstraAlgorithm.o: DijkstraAlgorithm.cpp Vertex.hpp Edge.hpp Graph.hpp
	$(CXX) $(CXXFLAGS) -c DijkstraAlgorithm.cpp -o DijkstraAlgorithm.o

Point.o:Point.cpp Point.hpp
	$(CXX) $(CXXFLAGS) -c Point.cpp -o Point.o

LinearApproximationReferenceAlgorithm.o: LinearApproximationReferenceAlgorithm.cpp LinearApproximationReferenceAlgorithm.hpp
	$(CXX) $(CXXFLAGS) -c LinearApproximationReferenceAlgorithm.cpp -o LinearApproximationReferenceAlgorithm.o

clean:
	rm $(OBJS) $(TARGET)
