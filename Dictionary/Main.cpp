#include "Dictionary.h"

using namespace Dictionary;

int main()
{
	setlocale(0, "");
	ifstream FileVoc{};
	string FileName{ "File.txt" };
	map<string, unsigned> voc{};
	std::cout << "ֲגוהטעו טלÿ פאיכא>>";
	getline(cin, FileName);
	FileVoc.open(FileName);
	Vocabulary a1;
	a1.FindWords(FileVoc);
	OutputSet(a1.GetVocabularyPtr());
	return 0;
}