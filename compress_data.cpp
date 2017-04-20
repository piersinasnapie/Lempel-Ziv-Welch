#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

/*
	Program kompresujący plik wejściowy do pliku binarnego 
	wykorzystujący metodę Lempel-Ziv-Welch. 

	c - pierwszy symbol wejściowy
	s - obecny symbol wejściowy
	cs - ciąg zbudowany z symboli c i s;   "cs = c+s"
	dictionary - słownik wyrazów/symboli
	
	* Słownik to mapa, w której kluczem jest ciąg znaków, a wartością liczba całkowita
	np. znak A ma kod 65, B:66 itd * 

	Algorytm w pseudokodzie:

	1. Zainicjuj słownik podstawowymi znakami (w tym przypadku 256 znakami ASCII)
	2. Wczytaj c
	3. Dopóki dane są na wejściu

		• Wczytaj znak s 
		• jeśli ciąg cs jest w słowniku
			-> c = cs
		• jeśli nie
			-> wypisz kod dla c(c znajduje się w słowniku)
			-> dodaj do słownika cs
			-> c = s
	4. Wypisz kod związany z c
 */



/*
	input_file - plik przesłany do skompresowania
	compressed_output - plik skompresowany
 */ 
void compress_data(istream &input_file,ostream &compressed_output) 
{
	// vector<char> - reprezentacja ciągu znaków
	// w rzeczywistości Map(String,Integer)
	map<vector<char>, int> dictionary;
	
	/* Inicjowanie słownika podstawowymi kodami ASCII */
	int dictionary_size = 256;
	for (int i=0; i<dictionary_size; ++i)     
	{   
		vector<char> symbol; // wektor 1-elementowy, używane do poprawnego wypełnienia słownika
		symbol.push_back( (char)i );
        dictionary[symbol] = i; 
	}

	vector<char> currentCode; // c
	char ch;
	input_file.get(ch);
	currentCode.push_back(ch); // wczytano c
	char nextCode; // s 

	/*
		dopóki dane są na wejściu:
		czytamy z pliku(input_file)  pojedyczny znak "nextCode"
	 */
	while( input_file.get(nextCode) ) 
	{
		currentCode.push_back(nextCode);
		if( dictionary.count(currentCode) == 0)// jeśli nie ma słowa w słowniku
		{ 
			// dodajemy do słownika nowy ciąg - cs
			dictionary[currentCode] = dictionary_size; 

			// teraz musimy wypisać kod słowa c
			// usuwamy ze słowa cs ciąg s
			currentCode.pop_back();

			// wypisujemy na wyjście kod dla c - do pliku
			compressed_output.write((char*)(&dictionary[currentCode]), sizeof(int));

			// zgodnie z algorytmem c=s
			currentCode.clear(); 
			currentCode.push_back(nextCode); // c=s
			dictionary_size++; // zwiększamy rozmiar słownika
		}
	}

	// Wypisz kod związany z c
	if( !currentCode.empty() )
		compressed_output.write((char*)(&dictionary[currentCode]), sizeof(unsigned int));
}

string setCompressedFileName(string fileName)
{
	string compressedFileName;
	compressedFileName = fileName;
	compressedFileName = compressedFileName.substr(0, compressedFileName.size()-4);
	compressedFileName.append(".lzw");
	return compressedFileName;
} 

void saveFileExtension(string extension, ostream & compressed_file)
{	
	string file_extension = extension;
	file_extension = file_extension.substr(file_extension.size()-3, file_extension.size());
	for(char c: file_extension)
		compressed_file.write((char*)&c, 1);
}


int main(int argc, const char* argv[]) 
{
	if(argc != 2)
	{
		cout << "Podano niepoprawną ilość plików." << endl;
		return -1;
	}

	ifstream input_file(argv[1], ios_base::binary);
	string compressed_file_name = setCompressedFileName(argv[1]);

	// plik skompresowany będzie miał nazwę z rozszerzeniem .lzw
	ofstream compressed_file(compressed_file_name, ios_base::binary);

	if(input_file.is_open() && compressed_file.is_open())
	{
		saveFileExtension(argv[1], compressed_file);
		compress_data(input_file, compressed_file);
	}
	else if(!input_file.good())
	{
		cout << "Podano zły plik wejściowy" << endl;
		return -1;
	}
	else
	{
		cerr << "Błąd przy kompresji" << endl;
		return -1;
	}

	// zamykwanie plików
	input_file.close();
	compressed_file.close();
	return 0;
}
