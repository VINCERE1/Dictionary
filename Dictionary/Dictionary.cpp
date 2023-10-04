#include "Dictionary.h"

using namespace std;

void Dictionary::Vocabulary::GetSeparators(ifstream& stream)
{
	separators = "";//Обнуляем строку разделителей (для прохода нескольких текстов)
	set<char> symbols;//Контейнер set для облегчения сбора разниых разделителей
	string line{};//Переменная для считывания строк текста
	while (!(stream.eof()))//Пока не конец файла
	{
		getline(stream, line);//Считываем строку текста
		for (const auto& ch : line)//Проходимся по каждому символу в строке
		{
			if (!(isalpha(ch)) && (!(isdigit(ch))) && (ch != ' '))//Если не является буквой, цифрой или пробелом
			{
				symbols.insert(ch);//То вставляем в массив разделителей
			}
		}
	}
	for (const auto& elem : symbols)//Переносим из set в переменную класса (для работы с функциями поиска в строке)
	{
		if ((elem == '"') || (elem == '\\'))//Добавление обратного слэша для распознавания в строке спец символов
			separators += 92;
		separators += elem;
	}
	separators += ' ';//Добавляем пробел (разделитель)
	//stream.clear();
	stream.seekg(0);//Сбрасываем указатель чтения потока
}

bool Dictionary::isAbbreviation(const string& word)
{
	return (((std::isupper(word[0]) && std::isupper(word[word.size() - 1])))
		&& (word.size() != 1));
}

void Dictionary::CheckStream(const ifstream& stream)
{
	if (!(stream))//Проверка существования потока
		throw string("Поток не верен");
	if (!(stream.is_open()))//Проверка открыт ли поток
		throw string("Файл не открыт");
	if (stream.eof())//Проверка не пустой ли поток
		throw string("Файл пуст");
}

void Dictionary::HandleWord(std::string& str)
{
	if (isAbbreviation(str))//Если аббревиатура, то не меняем слово
		return;
	bool isNum{ true };//Переменная для проверки является ли номером
	for (const auto& i : str)//Цикл проходит по всему слову, и если оно полностью состоит из цифр оставляет isNum в положении true
		if (!(isdigit(i)))
			isNum = false;
	if (isNum)//Если оно оказалось номером, то это не слово (заканчиваем обработку)
	{
		str = "";
		return;
	}
	for (auto& i : str)//Если оказалось словом, то переводим все символы в нижний регистр
	{
		i = std::tolower(i);
	}
}

void Dictionary::Vocabulary::FindWords(ifstream& stream, const char& letter)
{
	try
	{
		CheckStream(stream);//Проверка потока
		if (!(isalpha(letter)) && (letter != '.'))//Проверка введенного символа начала слова
			throw string("Неверный ввод символа начала слова");
		bool SymbolReceived{ letter != '.' };//Переменная для определения поиска слов
		GetSeparators(stream);//Получения из поток всех возможных разделителей
		string text{};//Переменная для записи текста из потока
		string word{};//Переменная для записи слова
		while (!(stream.eof()))
		{
			getline(stream, text);//Получаем строку текста из потока
			size_t start{};//Индекс начала слова
			size_t end{};//Индекс конца слова
			erase(text, '\n');//Убираем из текста символы переноса строки
			while (end < text.length())
			{
				start = text.find_first_not_of(separators, end);//Находим первый символ не из разделителей (начало слова)
				if (start == string::npos)//Если такой не нашелся значит строка пуста, выходим из цикла
					break;
				end = text.find_first_of(separators, start + 1);//Находим первый разделитель после начала слова
				if (end == string::npos)//Если не найден, то задаем значение длины текста
					end = text.length();
				if ((text[start] == letter) || (!SymbolReceived))//Проверяем первую букву или заданное условие (символ поиска получен)
				{
					word = text.substr(start, end - start);//Вырезаем слово из текста
					HandleWord(word);//Обрабатываем его
					if (word != "")
						voc_set->insert(word);
				}
			}
		}
	}
	catch (const out_of_range& err)
	{
		cout << err.what() << endl;
	}
	catch (const string& err)
	{
		cout << err << endl;
	}
}

void Dictionary::Vocabulary::FindWordsCount(ifstream& stream, const char& letter)//То же, что и FindWords только с подсчетом слов
{
	try
	{
		CheckStream(stream);
		if (!(isalpha(letter)) && (letter != '.'))
			throw string("Неверный ввод символа начала слова");
		bool SymbolReceived{ letter != '.' };
		GetSeparators(stream);
		string text{};
		string word{};
		while (!(stream.eof()))
		{
			getline(stream, text);
			size_t start{};
			size_t end{};
			erase(text, '\n');
			while (end < text.length())
			{
				start = text.find_first_not_of(separators, end);
				if (start == string::npos)
					break;
				end = text.find_first_of(separators, start + 1);
				if (end == string::npos)
					end = text.length();
				if ((text[start] == letter) || (!SymbolReceived))
				{
					word = text.substr(start, end - start);
					HandleWord(word);
					if (word != "")
						if (voc_map->contains(word))//Если слово уже есть увеличиваем счетчик
							voc_map->at(word)++;
						else//Если слова нет, то добавляем его и задаем кол-во 1
							voc_map->insert({ word,1 });
				}
			}
		}
	}
	catch (const out_of_range& err)
	{
		cout << err.what() << endl;
	}
	catch (const string& err)
	{
		cout << err << endl;
	}
}

Dictionary::Vocabulary::~Vocabulary()
{
	delete voc_set;
	delete voc_map;
}

void Dictionary::OutputSet(const set<string>* list)
{
	if (list->empty())
	{
		std::cout << "Словарь пуст" << endl;
		return;
	}
	for (auto iter{ list->begin() }; iter != list->end(); iter++)
		cout << *iter << endl;
}

void Dictionary::OutputMap(const std::map<string, unsigned>* voc)
{
	if (!(voc->empty()))
		for (auto iter{ voc->begin() }; iter != voc->end(); iter++)
		{
			std::cout << iter->first << " - " << iter->second << " раз" << std::endl;
		}
	else
		std::cout << "Слов не было найдено" << std::endl;
}

Dictionary::Vocabulary::Vocabulary()
{
	voc_map = new map<string, unsigned>{};
	voc_set = new set<string>{};
}

map<string, unsigned> const* Dictionary::Vocabulary::GetVocabularyMapPtr() const
{
	return voc_map;
}

set<string> const* Dictionary::Vocabulary::GetVocabularyPtr() const
{
	return voc_set;
}

