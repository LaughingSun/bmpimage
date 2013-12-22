test: bmpimage.h test.h test.cpp lsb_encoder.h
	g++  -O2 test.cpp -o test
	./test

debug_test: bmpimage.h test.h test.cpp
	g++ -g test.cpp -o test
	gdb test

lsb: lsb_encoder.h lsb_decoder.h lsb_encoder.cpp lsb_decoder.cpp
	g++ -O2 lsb_encoder.cpp -o lsb_encoder
	g++ -O2 lsb_decoder.cpp -o lsb_decoder

debug_lsb:
	g++ -g lsb_encoder.cpp -o lsb_encoder
	g++ -g lsb_decoder.cpp -o lsb_decoder

dump: dump.cpp bmpimage.h
	g++ -O2 dump.cpp -o dump 

clean:
	rm -rf test dump lsb_encoder *\.gch *\.dSYM

