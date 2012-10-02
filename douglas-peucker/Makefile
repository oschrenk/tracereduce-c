CXX=g++
CXXFLAGS=-g -Wall
TARGET=lab

OBJS=main.o
OBJS+=Point.o
OBJS+=DouglasPeuckerReferenceAlgorithm.o
OBJS+=EuclideanGeometry.o

all:$(TARGET)
$(TARGET):$(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

main.o:main.cpp Geometry.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

Point.o:Point.cpp Point.hpp
	$(CXX) $(CXXFLAGS) -c Point.cpp -o Point.o
	
DouglasPeuckerReferenceAlgorithm.o:DouglasPeuckerReferenceAlgorithm.cpp DouglasPeuckerReferenceAlgorithm.hpp Geometry.hpp
	$(CXX) $(CXXFLAGS) -c DouglasPeuckerReferenceAlgorithm.cpp -o DouglasPeuckerReferenceAlgorithm.o

EuclideanGeometry.o:EuclideanGeometry.cpp EuclideanGeometry.hpp Geometry.hpp
	$(CXX) $(CXXFLAGS) -c EuclideanGeometry.cpp -o EuclideanGeometry.o

clean:
	rm $(OBJS) lab
