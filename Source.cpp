#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <ctime>
#include <random>
#include <algorithm>

using namespace std;

/*	� ���� �� ���� ������� ������� ������� ��� ����������� ����������� -
	�����, ���, ������ ��������� �����. �� ������, �� �����, � �����
	�������� ���� ���������� ������ ��������������, �������.
	�� ���� � ��������� ����� �����-�� ����� �����, � ������� � ���� ��������.	*/

void greetings() {
	cout << "�������� � ������� vocabulary v0.1 ���� ������������ ��� � ������ ��� ��������� �������������������!\n\n";
}

void print_error_0() {
	cout << "��������� ������!" << endl;
	cout << "��� ������ 0: ���� �� ������." << endl;
	cout << "" << endl;
	cout << "���������, ��� � ����� � ���������� ������� ��������� �����:" << endl;
	cout << "DICT.txt" << endl;
	cout << "" << endl;
	cout << "� ������ ���������� ������ �������� ����." << endl;
	cout << "" << endl;
	system("pause");
}

void print_error_1() {
	cout << "" << endl;
	cout << "��� ������ 1: ������ �� ������." << endl;
	cout << "���������, ��� �� ����� ������������ ���������." << endl;
	cout << "��� ������� �� ������������� � ��������, �� ������� ������� �������." << endl;
	cout << "" << endl;
}

void print_contents(map <string, int>& contents) {
	for (auto a : contents) {
		cout << a.first << " " << a.second << endl;
	}
}

string get(string cache, int pos, bool second) {

	string result;

	// ��������� ������� / ���������� �����
	if (second) {
		result = cache.substr(pos + 2);
	}
	else {
		result = cache.substr(0, pos - 1);
	}

	// ������������� ������
	result.erase(remove(result.begin(), result.end(), '\t'), result.end());
	if (result[result.size()] == ' ') {
		result.pop_back();
	}

	return result;
}

string clean_content(string cache) {
	
	cache.erase(remove(cache.begin(), cache.end(), '='), cache.end());
	cache = cache.substr(1, cache.size() - 2);

	return cache;
}

string user_choice(map <string, int>& contents) {
	
	string cache;

	print_contents(contents);
	cout << "�������� ������ �� ������ ���������: ";

	getline(cin, cache);

	transform(cache.begin(), cache.end(), cache.begin(), ::toupper);
	while (contents.find(cache) == contents.end()) {
		print_error_1();
		cout << "�������� ������ �� ������ ���������: ";
		getline(cin, cache);
		transform(cache.begin(), cache.end(), cache.begin(), ::toupper);
	}

	cout << endl;
	return cache;
}

void run(map <string, string>& dict) {
	// � �������-�� �� ������� ���!
	string nothing;

	for (auto i : dict) {
		cout << "Q: " << i.first << endl;
		cout << "Y: ";
		getline(cin, nothing);
		cout << "A: " << i.second << endl;
		cout << endl;
	}
}

int main() {
	srand(static_cast<unsigned int>(time(0)));
	setlocale(LC_ALL, "Russian_Russia.1251");

	ifstream in(".\\DICT.txt");
	if (in.is_open()) {

		greetings();
		map <string, int> contents;
		map <string, map <string, string>> file;
		string cache,
			cache_rus,
			cache_eng,
			last = "",
			prev = "";

		int line = -5,
			pos;

		map <string, string> dict;
		while (getline(in, cache)) {

			pos = cache.find("=");
			if (pos != string::npos) {
				prev = last;
				last = clean_content(cache);
			}

			if (line >= 0) {
				pos = cache.find("�");

				if (pos != string::npos) {

					cache_rus = get(cache, pos, 0);
					cache_eng = get(cache, pos, 1);
					dict[cache_rus] = cache_eng;

				}
				else {

					pos = cache.find("=");
					if (pos != string::npos) {
						contents[prev] = line;
						file[prev] = dict;
						map <string, string> dict;
						line = -1;
					}
					else {
						line--;
					}

				}

			}

			line++;
		}
		
		contents[last] = line;

		cache = user_choice(contents);
		run(file[cache]);

		cache = "";
		while (true) {
			cout << "���������� �������? (Y/N): ";
			cin >> cache;
			
			if (cache == "Y") {
				cache = user_choice(contents);
				run(file[cache]);
				continue;
			}
			else if (cache == "N") {
				cout << "�������, ��� �� ����." << endl;
				return 0;
			}
			else {
				cout << "�������������� �������" << endl;
				return 0;
			}
		}
	}
	else {
		print_error_0();
	}

	in.close();

}
