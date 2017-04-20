DIRECTORY = Projekt

FLAGS = g++ -ansi -Wall -pedantic -std=c++11

all: compress_data decompress_data

compress_data: compress_data.cpp
	$(FLAGS) $^ -o $@.x
	
runc: compress_data
	./compress_data.x konstytucja.doc

decompress_data: decompress_data.cpp
	$(FLAGS) $^ -o $@.x

rund: decompress_data
	./decompress_data.x konstytucja.lzw data

compare: 
	diff -s konstytucja.doc data.doc

clean:
	rm -f *.x *.dat *.lzw data.doc

compress: clean
	(cd ../; tar -cvzf $(DIRECTORY).tar.gz $(DIRECTORY))

.PHONY: clean compress all
	