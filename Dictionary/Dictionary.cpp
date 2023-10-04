#include "Dictionary.h"

using namespace std;

void Dictionary::Vocabulary::GetSeparators(ifstream& stream)
{
	separators = "";//�������� ������ ������������ (��� ������� ���������� �������)
	set<char> symbols;//��������� set ��� ���������� ����� ������� ������������
	string line{};//���������� ��� ���������� ����� ������
	while (!(stream.eof()))//���� �� ����� �����
	{
		getline(stream, line);//��������� ������ ������
		for (const auto& ch : line)//���������� �� ������� ������� � ������
		{
			if (!(isalpha(ch)) && (!(isdigit(ch))) && (ch != ' '))//���� �� �������� ������, ������ ��� ��������
			{
				symbols.insert(ch);//�� ��������� � ������ ������������
			}
		}
	}
	for (const auto& elem : symbols)//��������� �� set � ���������� ������ (��� ������ � ��������� ������ � ������)
	{
		if ((elem == '"') || (elem == '\\'))//���������� ��������� ����� ��� ������������� � ������ ���� ��������
			separators += 92;
		separators += elem;
	}
	separators += ' ';//��������� ������ (�����������)
	//stream.clear();
	stream.seekg(0);//���������� ��������� ������ ������
}

bool Dictionary::isAbbreviation(const string& word)
{
	return (((std::isupper(word[0]) && std::isupper(word[word.size() - 1])))
		&& (word.size() != 1));
}

void Dictionary::CheckStream(const ifstream& stream)
{
	if (!(stream))//�������� ������������� ������
		throw string("����� �� �����");
	if (!(stream.is_open()))//�������� ������ �� �����
		throw string("���� �� ������");
	if (stream.eof())//�������� �� ������ �� �����
		throw string("���� ����");
}

void Dictionary::HandleWord(std::string& str)
{
	if (isAbbreviation(str))//���� ������������, �� �� ������ �����
		return;
	bool isNum{ true };//���������� ��� �������� �������� �� �������
	for (const auto& i : str)//���� �������� �� ����� �����, � ���� ��� ��������� ������� �� ���� ��������� isNum � ��������� true
		if (!(isdigit(i)))
			isNum = false;
	if (isNum)//���� ��� ��������� �������, �� ��� �� ����� (����������� ���������)
	{
		str = "";
		return;
	}
	for (auto& i : str)//���� ��������� ������, �� ��������� ��� ������� � ������ �������
	{
		i = std::tolower(i);
	}
}

void Dictionary::Vocabulary::FindWords(ifstream& stream, const char& letter)
{
	try
	{
		CheckStream(stream);//�������� ������
		if (!(isalpha(letter)) && (letter != '.'))//�������� ���������� ������� ������ �����
			throw string("�������� ���� ������� ������ �����");
		bool SymbolReceived{ letter != '.' };//���������� ��� ����������� ������ ����
		GetSeparators(stream);//��������� �� ����� ���� ��������� ������������
		string text{};//���������� ��� ������ ������ �� ������
		string word{};//���������� ��� ������ �����
		while (!(stream.eof()))
		{
			getline(stream, text);//�������� ������ ������ �� ������
			size_t start{};//������ ������ �����
			size_t end{};//������ ����� �����
			erase(text, '\n');//������� �� ������ ������� �������� ������
			while (end < text.length())
			{
				start = text.find_first_not_of(separators, end);//������� ������ ������ �� �� ������������ (������ �����)
				if (start == string::npos)//���� ����� �� ������� ������ ������ �����, ������� �� �����
					break;
				end = text.find_first_of(separators, start + 1);//������� ������ ����������� ����� ������ �����
				if (end == string::npos)//���� �� ������, �� ������ �������� ����� ������
					end = text.length();
				if ((text[start] == letter) || (!SymbolReceived))//��������� ������ ����� ��� �������� ������� (������ ������ �������)
				{
					word = text.substr(start, end - start);//�������� ����� �� ������
					HandleWord(word);//������������ ���
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

void Dictionary::Vocabulary::FindWordsCount(ifstream& stream, const char& letter)//�� ��, ��� � FindWords ������ � ��������� ����
{
	try
	{
		CheckStream(stream);
		if (!(isalpha(letter)) && (letter != '.'))
			throw string("�������� ���� ������� ������ �����");
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
						if (voc_map->contains(word))//���� ����� ��� ���� ����������� �������
							voc_map->at(word)++;
						else//���� ����� ���, �� ��������� ��� � ������ ���-�� 1
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
		std::cout << "������� ����" << endl;
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
			std::cout << iter->first << " - " << iter->second << " ���" << std::endl;
		}
	else
		std::cout << "���� �� ���� �������" << std::endl;
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

