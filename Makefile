.PHONY: all build clean deepclean

# build cmake
all:
	+make -C build

# run the cmake configure process
build:
	mkdir -p build
	cmake -B`pwd`/build -H`pwd`

# clean the build directory
# and delete all test output files
clean:
	+make -C build clean

# delete all CMakeFiles and start from scratch
deepclean:
	-rm -rf build/*
