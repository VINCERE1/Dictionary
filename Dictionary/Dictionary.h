#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <span>
#include <fstream>
#include <stdlib.h>

using namespace std;

namespace Dictionary
{
	class Vocabulary
	{
	public:
		Vocabulary();

		map<string, unsigned> const* GetVocabularyMapPtr() const;//Метод получения константного указателя на словарь (map)
		set<string> const* GetVocabularyPtr() const;//Метод получения константного указателя на словарь (set)

		void FindWords(ifstream& stream, const char& letter = '.');//Метод поиска слов в тексте по потоку с возможностью задания первой буквы (регистр важен)
		void FindWordsCount(ifstream& stream, const char& letter = '.');//Метод подсчета вхождений слов в тексте по потоку с возможностью задания первой буквы
		void GetSeparators(ifstream& stream);//Метод получения всех разделителей в тексте по потоку

		~Vocabulary();
	private:
		map<string, unsigned>* voc_map;//Указатель на map с подсчетом слов
		set<string>* voc_set;//Указатель на set словарь
		string separators{ "" };//Переменная для хранения разделителей текста
	};
	bool isAbbreviation(const string& word);//Проверка является ли аббревиатурой

	void CheckStream(const ifstream& stream);//Проверка потока на валидность
	void HandleWord(string& str);//Обработка слова (приведение символов к нижнему регистру, если не аббревиатура)

	void OutputMap(const map<string, unsigned>* voc);//Функция вывода контейнера map (подсчет слов)
	void OutputSet(const set<string>* list);//Функция вывода array-подобных контейнеров
}