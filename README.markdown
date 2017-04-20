
Projekt
=======

Aby skompilować programy należy wpisać w konsoli: make all

Aby uruchomić demonstrację działania programów należy wpisać:

	make runc

	make rund

	make compare

Kolejno zostanie skompresowany plik przykładowy do pliku z rozszerzeniem .lzw
Oraz rozpakowany do pliku z podstawowym rozszerzeniem. Na końcu sprawdzamy, czy plik po rozpakowaniu jest taki sam jak przed spakowaniem.

Aby skompresować inny plik należy wpisać:

	./compress_data plik.dat

	plik.dat to plik podany do skomrpesowania

Aby rozpakować plik należy wpisać

	./decompress_data plik.lzw nowa_nazwa_pliku
