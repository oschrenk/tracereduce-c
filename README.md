# README #

## Development ##

Building [boost](http://www.boost.org/) from svn (current distribution doesn't include geometry extensions)

	cd projects/external
	git clone https://github.com/ryppl/boost-svn boost
	cd boost
	./bootstrap.sh --prefix=~/Development/libs/boost/
	./b2 link=static threading=multi toolset=darwin cxxflags="-arch i386 -arch x86_64" stage