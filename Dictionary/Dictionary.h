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

		map<string, unsigned> const* GetVocabularyMapPtr() const;//����� ��������� ������������ ��������� �� ������� (map)
		set<string> const* GetVocabularyPtr() const;//����� ��������� ������������ ��������� �� ������� (set)

		void FindWords(ifstream& stream, const char& letter = '.');//����� ������ ���� � ������ �� ������ � ������������ ������� ������ ����� (������� �����)
		void FindWordsCount(ifstream& stream, const char& letter = '.');//����� �������� ��������� ���� � ������ �� ������ � ������������ ������� ������ �����
		void GetSeparators(ifstream& stream);//����� ��������� ���� ������������ � ������ �� ������

		~Vocabulary();
	private:
		map<string, unsigned>* voc_map;//��������� �� map � ��������� ����
		set<string>* voc_set;//��������� �� set �������
		string separators{ "" };//���������� ��� �������� ������������ ������
	};
	bool isAbbreviation(const string& word);//�������� �������� �� �������������

	void CheckStream(const ifstream& stream);//�������� ������ �� ����������
	void HandleWord(string& str);//��������� ����� (���������� �������� � ������� ��������, ���� �� ������������)

	void OutputMap(const map<string, unsigned>* voc);//������� ������ ���������� map (������� ����)
	void OutputSet(const set<string>* list);//������� ������ array-�������� �����������
}