#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <ctime>
#include <random>
#include <algorithm>

using namespace std;

/*	У меня не было времени сделать словарь без определённых ограничений -
	таких, как, формат исходного файла. Он жёсткий, он тупой, а также
	исходный файл приходится делать самостоятельно, вручную.
	Но если у программы будет какой-то смысл жизни, я займусь и этим вопросом.	*/

void greetings() {
	cout << "Помощник в зубрёжке vocabulary v0.1 бета приветствует вас и желает вам приятного времяпрепровождения!\n\n";
}

void print_error_0() {
	cout << "Произошла ошибка!" << endl;
	cout << "Код ошибки 0: Файл не найден." << endl;
	cout << "" << endl;
	cout << "Убедитесь, что в папке с программой имеются следующие файлы:" << endl;
	cout << "DICT.txt" << endl;
	cout << "" << endl;
	cout << "В случае повторения ошибки сломайте стол." << endl;
	cout << "" << endl;
	system("pause");
}

void print_error_1() {
	cout << "" << endl;
	cout << "Код ошибки 1: Раздел не найден." << endl;
	cout << "Убедитесь, что вы ввели наименование правильно." << endl;
	cout << "Имя раздела не чувствительно к регистру, но требует каждого символа." << endl;
	cout << "" << endl;
}

void print_contents(map <string, int>& contents) {
	for (auto a : contents) {
		cout << a.first << " " << a.second << endl;
	}
}

string get(string cache, int pos, bool second) {

	string result;

	// выцепляем русскую / английскую часть
	if (second) {
		result = cache.substr(pos + 2);
	}
	else {
		result = cache.substr(0, pos - 1);
	}

	// косметические правки
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
	cout << "Выберите раздел из списка доступных: ";

	getline(cin, cache);

	transform(cache.begin(), cache.end(), cache.begin(), ::toupper);
	while (contents.find(cache) == contents.end()) {
		print_error_1();
		cout << "Выберите раздел из списка доступных: ";
		getline(cin, cache);
		transform(cache.begin(), cache.end(), cache.begin(), ::toupper);
	}

	cout << endl;
	return cache;
}

void run(map <string, string>& dict) {
	// А рандома-то не завезли ещё!
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
				pos = cache.find("—");

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
			cout << "Продолжить зубрёжку? (Y/N): ";
			cin >> cache;
			
			if (cache == "Y") {
				cache = user_choice(contents);
				run(file[cache]);
				continue;
			}
			else if (cache == "N") {
				cout << "Спасибо, что вы есть." << endl;
				return 0;
			}
			else {
				cout << "необработанный эксепшн" << endl;
				return 0;
			}
		}
	}
	else {
		print_error_0();
	}

	in.close();

}
