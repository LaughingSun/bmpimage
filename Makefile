test: bmpimage.h test.h test.cpp
	g++ test.cpp -o test
	./test

clean:
	rm -rf test
