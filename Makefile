test: bmpimage.h test.h test.cpp
	g++  -O2 test.cpp -o test
	./test

debug_test: bmpimage.h test.h test.cpp
	g++ -g test.cpp -o test
	gdb test

dump: dump.cpp bmpimage.h
	g++ -O2 dump.cpp -o dump 

clean:
	rm -rf test dump *\.gch *\.dSYM

