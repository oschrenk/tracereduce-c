CXX=g++
CXXFLAGS=-g -O2 -Wall -ansi -Wno-deprecated
TARGET=douglaPeucker-opencl

#BOOST_HOME = /Users/university/local/include
#BOOST_INCLUDE = /Users/university/local/include
#BOOST_LIB = /Users/university/local/lib

BOOST_INCLUDE = /usr/local/boost_1_49_0
BOOST_LIB = /usr/local/boost_1_49_0/stage/lib

OPENCLSDK_INCLUDE = /opt/AMDAPP/include
OPENCLSDK_LIB =  /opt/AMDAPP/lib/x86

INCLUDE= -I $(BOOST_INCLUDE) -I $(OPENCLSDK_INCLUDE)
LIBS= -L $(BOOST_LIB) -L $(OPENCLSDK_LIB)

LDFLAGS = -lboost_system
LDFLAGS += -lboost_date_time
LDFLAGS += -lboost_filesystem
LDFLAGS += -lboost_graph
LDFLAGS += -lOpenCL

OBJS=main.o
OBJS+=Point.o
OBJS+=OpenClUtils.o
OBJS+=DeviceHandler.o
OBJS+=LinearOptimumOpenClAlgorithm.o


all:$(TARGET)
$(TARGET):$(OBJS)
	$(CXX) $(OBJS) $(LIBS) $(LDFLAGS) -o $(TARGET)

main.o: main.cpp 
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c main.cpp -o main.o

Point.o:Point.cpp Point.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c Point.cpp -o Point.o
	
LinearOptimumOpenClAlgorithm.o: LinearOptimumOpenClAlgorithm.cpp LinearOptimumOpenClAlgorithm.hpp 
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c LinearOptimumOpenClAlgorithm.cpp -o LinearOptimumOpenClAlgorithm.o

HaversineDistance.o:HaversineDistance.cpp HaversineDistance.hpp Distance.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c HaversineDistance.cpp -o HaversineDistance.o
	
OpenClUtils.o: OpenClUtils.cpp OpenClUtils.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c OpenClUtils.cpp -o OpenClUtils.o
	
DeviceHandler.o: DeviceHandler.cpp  DeviceHandler.hpp 
	$(CC) $(CFLAGS) $(INCLUDE) -c DeviceHandler.cpp -o DeviceHandler.o
	
clean:
	rm $(OBJS) $(TARGET)
