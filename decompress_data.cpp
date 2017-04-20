#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <map>

using namespace std;

/*
	Program dekompresujący plik z rozszerzeniem .lzw 
	oparty o algorytm dekodowania Lempel-Ziv-Welch

	Przy dekodowaniu zmieniamy strukturę słownika

	Algorytm w pseudokodzie:

	1.Wypełnij słownik alfabetem źródła informacji
	2. pk - pierwszy kod skompresowanych danych
	3. Wypisz na wyjście kod związany z pk, czyli dictionary[pk]
	4. Dopóki są słowa na wejściu

		• Wczytaj kod k
		• pc (poprzedni kod) = dictionary[pk]
		• Jeśli k jest w słowniku 
			-> dodaj do słownika pc + pierwszy symbol dictionary[k]
			-> wypisz na wyjście ciąg dla k, czyli dictionary[k]
		• Jeśli nie
			-> dodaj do słownika pc + pierwszy symbol dictionary[pc]
			-> wypisza na wyjście dictionary[pc]
		• pk = k
 */

void decompress(istream &is, ostream &os) 
{
	int dictionary_size = 256;
	map<int, vector<char>> dictionary;
	// vector<vector<char>> dictionary;
	for(int i=0; i<dictionary_size; i++)
	{
		vector<char> v;
		v.push_back( (char)i );
		dictionary[i] = v;
	}

	vector<char> code;
	unsigned int codeKey;

	while( is.read((char*)(&codeKey),4) ) 
	{ 		
		if(codeKey > dictionary.size()) // znaku nie ma w podstawowym słowniku
		{
			cout << "Błąd podczas dekodowania" << endl;
			return;
		}	
		else if (codeKey == dictionary.size()) // słowa nie ma słowniku 
		{
			code.push_back(code.front()); // pc + pc.charAt(0);
			dictionary[dictionary_size++] = code;
		}
	
		else if (!code.empty()) // słowo występuje w słowniku
		{
			code.push_back(dictionary.at(codeKey).front()); // pc + dictionary[k]
			dictionary[dictionary_size++] = code;
		}
			
		os.write( &dictionary[codeKey].front(), dictionary[codeKey].size() );
		code = dictionary[codeKey];

	}
	if(!is.eof() || is.gcount() != 0) //drugi warunek sprawdza czy ostatnio jakiekolwiek znaki z pliku zostaly zmienione
		cerr << "corrupted input file\n";
}

bool goodExtension(string fileName)
{
	string extension = fileName;
	extension = extension.substr(extension.size()-3, extension.size());
	return extension == "lzw";
}

string setFileExtension(istream & compressed_file)
{
	string extension = ".";
	char chars[3];
	for(int i=0; i<3; i++)
	{
		compressed_file.read((char*)(&chars[i]), 1);
		extension.push_back(chars[i]);
	}
	return extension;
}

int main(int argc, char* argv[]) 
{
	if(argc != 3)
	{
		cout << "Podano nieprawidłową ilość plików." << endl;
		return -1;
	}

	ifstream compressed_file(argv[1], ios_base::binary);

	if( !goodExtension(argv[1]) )
	{
		cout << "Plik do dekompresji musi posiadać rozszerzenie: .lzw " << endl;
		return -1;
	}

	string file_name = argv[2];
	file_name.append(setFileExtension(compressed_file));

	ofstream decompressed_file(file_name, ios_base::binary);
	if( compressed_file.is_open() && decompressed_file.is_open() )
	{
		decompress(compressed_file, decompressed_file);
	}
	else if(!compressed_file.is_open())
		cerr << "Error while loading input file\n";
	else
		cerr << "Error while loading output file\n";
	return 0;
}
