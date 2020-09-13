#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <ctime>
#include <random>
#include <algorithm>

using namespace std;

/*	Ó ìåíÿ íå áûëî âðåìåíè ñäåëàòü ñëîâàðü áåç îïðåäåë¸ííûõ îãðàíè÷åíèé -
	òàêèõ, êàê, ôîðìàò èñõîäíîãî ôàéëà. Îí æ¸ñòêèé, îí òóïîé, à òàêæå
	èñõîäíûé ôàéë ïðèõîäèòñÿ äåëàòü ñàìîñòîÿòåëüíî, âðó÷íóþ.
	Íî åñëè ó ïðîãðàììû áóäåò êàêîé-òî ñìûñë æèçíè, ÿ çàéìóñü è ýòèì âîïðîñîì.	*/

void greetings() {
	cout << "Ïîìîùíèê â çóáð¸æêå vocabulary v0.1 áåòà ïðèâåòñòâóåò âàñ è æåëàåò âàì ïðèÿòíîãî âðåìÿïðåïðîâîæäåíèÿ!\n\n";
}

void print_error_0() {
	cout << "Ïðîèçîøëà îøèáêà!" << endl;
	cout << "Êîä îøèáêè 0: Ôàéë íå íàéäåí." << endl;
	cout << "" << endl;
	cout << "Óáåäèòåñü, ÷òî â ïàïêå ñ ïðîãðàììîé èìåþòñÿ ñëåäóþùèå ôàéëû:" << endl;
	cout << "DICT.txt" << endl;
	cout << "" << endl;
	cout << "Â ñëó÷àå ïîâòîðåíèÿ îøèáêè ñëîìàéòå ñòîë." << endl;
	cout << "" << endl;
	system("pause");
}

void print_error_1() {
	cout << "" << endl;
	cout << "Êîä îøèáêè 1: Ðàçäåë íå íàéäåí." << endl;
	cout << "Óáåäèòåñü, ÷òî âû ââåëè íàèìåíîâàíèå ïðàâèëüíî." << endl;
	cout << "Èìÿ ðàçäåëà íå ÷óâñòâèòåëüíî ê ðåãèñòðó, íî òðåáóåò êàæäîãî ñèìâîëà." << endl;
	cout << "" << endl;
}

void print_contents(map <string, int>& contents) {
	for (auto a : contents) {
		cout << a.first << " " << a.second << endl;
	}
}

string get(string cache, int pos, bool second) {

	string result;

	// âûöåïëÿåì ðóññêóþ / àíãëèéñêóþ ÷àñòü
	if (second) {
		result = cache.substr(pos + 2);
	}
	else {
		result = cache.substr(0, pos - 1);
	}

	// êîñìåòè÷åñêèå ïðàâêè
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
	cout << "Âûáåðèòå ðàçäåë èç ñïèñêà äîñòóïíûõ: ";

	getline(cin, cache);

	transform(cache.begin(), cache.end(), cache.begin(), ::toupper);
	while (contents.find(cache) == contents.end()) {
		print_error_1();
		cout << "Âûáåðèòå ðàçäåë èç ñïèñêà äîñòóïíûõ: ";
		getline(cin, cache);
		transform(cache.begin(), cache.end(), cache.begin(), ::toupper);
	}

	cout << endl;
	return cache;
}

void run(map <string, string>& dict) {
	// À ðàíäîìà-òî íå çàâåçëè åù¸!
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
			cout << "Ïðîäîëæèòü çóáð¸æêó? (Y/N): ";
			cin >> cache;
			
			if (cache == "Y") {
				cache = user_choice(contents);
				run(file[cache]);
				continue;
			}
			else if (cache == "N") {
				cout << "Ñïàñèáî, ÷òî âû åñòü." << endl;
				return 0;
			}
			else {
				cout << "íåîáðàáîòàííûé ýêñåïøí" << endl;
				return 0;
			}
		}
	}
	else {
		print_error_0();
	}

	in.close();
	//
}
