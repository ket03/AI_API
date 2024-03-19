.PHONY: install uninstall dist clean
CC=g++
CFLAGS=-Wall -Wextra -Werror -std=c++17

install: uninstall
	make clean
	mkdir build
	cd src && qmake && make && make clean && rm Makefile && cd ../ && mv src/GeoApi.app build

uninstall:
	rm -rf build*

dvi:
	open README.md

dist: install
	rm -rf Dist_GeoApi/
	mkdir Dist_GeoApi/
	mkdir Dist_GeoApi/src
	mv ./build/GeoApi.app Dist_GeoApi/src/
	tar cvzf Dist_GeoApi.tgz Dist_GeoApi/
	rm -rf Dist_GeoApi/
	rmdir build

clean:
	cd src && rm -rf *.a && rm -rf *.o  && rm -rf *.dSYM && rm -rf *.out && rm -rf $(EXECUTABLE) && rm -rf CPPLINT.cfg 
	cd src && rm -rf *.info && rm -rf Dist_GeoApi && rm -rf *tgz && rm -rf build && rm -rf .qmake.stash