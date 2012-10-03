# README #

## Development ##

Requirements

- [boost](http://www.boost.org/)
- [GNU make](http://www.gnu.org/software/make/)

Altough the quasi default package manager [homebrew](http://mxcl.github.com/homebrew/) for OSX supports boost, it somehow doesn't get integrated into the system.

Building [boost](http://www.boost.org/) for OSX 10.8.2 from svn (via git) and install it into `$HOME/local

	cd projects/external
	git clone https://github.com/ryppl/boost-svn boost
	cd boost
	./bootstrap.sh --with-toolset=clang --prefix=$HOME/local
	./b2 install toolset=clang cxxflags="-std=c++11 -stdlib=libc++ -arch i386 -arch x86_64" linkflags="-stdlib=libc++" target-os=darwin --address-model=32_64 threading=multi

To clean up temporary files

	./bjam --clean-all
	
I still got some compilation errors

	Undefined symbols for architecture x86_64:
	  "boost::system::system_category()", referenced from:
	      __static_initialization_and_destruction_0(int, int)in main.o
	  "boost::system::generic_category()", referenced from:
	      __static_initialization_and_destruction_0(int, int)in main.o
	ld: symbol(s) not found for architecture x86_64

I turned out I forgot to include `-lboost_system`. In my case:

	LDFLAGS = -lboost_system
	LDFLAGS += -lboost_date_time
	LDFLAGS += -lboost_filesystem
	LDFLAGS += -lboost_graph

I still got

	export DYLD_LIBRARY_PATH=$HOME/local/lib:$DYLD_LIBRARY_PATH