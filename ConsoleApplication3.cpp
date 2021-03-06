#include "pch.h"
#include <fstream>
#include <string>
#include <ctype.h>
#include <cmath>
#include <algorithm>
#include <map>
#include <iostream>
using namespace std;
char alph[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};

string encrypting(string source,int replace) {
	string crpt_text;
	for (int i = 0; i < source.length() - 1; i++) {
		bool flag = false;
		for (int j = 0; j < sizeof(alph); j++) {
			if (source[i] == alph[j]) {
				flag = true;
				int new_position = j + replace;
				if (new_position >= sizeof(alph)) {
					new_position -= sizeof(alph);
				}
				crpt_text += alph[new_position];
			}
		}
		if (!flag) {
			crpt_text += source[i];
		}
	}
	return crpt_text;
}

map <char, float> frequency_one(string source, map <char, float> letters) {
	string checked;
	for (int i = 0; i < source.length() - 1; i++) {
		for (int j = 0; j < sizeof(alph); j++) {
			if (source[i] == alph[j]) {
				if (checked.find(source[i]) == string::npos) {
					float count = 0;
					for (int k = 0; k < source.length() - 1; k++) {
						if (source[i] == source[k]) {
							count++;
						}
					}
					letters[source[i]] = count / (source.length() - 1);
					checked += source[i];
					checked += " ";
				}
			}
		}
	}
	return letters;
}

map <string, float> frequency_bi(string source, map <string, float> letters) {
string checked, curr, check;
	for (int i = 0; i < source.length() - 1; i++) {
		for (int j = 0; j < sizeof(alph); j++) {
			if (source[i] == alph[j]) {
				for (int c = 0; c < sizeof(alph); c++) {
					if (source[i + 1] == alph[c]) {
						curr = source[i];
						curr += source[i + 1];
						if (checked.find(curr) == string::npos) {
							float count = 0;
							for (int k = 0; k < source.length() - 1; k++) {
								check = source[k];
								check += source[k + 1];
								if (check == curr) {
									count++;
								}
							}
							letters[curr] = count / (source.length() - 1);
							checked += curr;
							checked += " ";
						}
						j, c = sizeof(alph);
					}
				}
			}
		}
	}
	return letters;
}

string decrypting_one(string source, map <char, float> letters, map <char, float> crypto_letters) {
	string decrypto_text;
	for (int i = 0; i < source.length(); i++) {
		bool flag = false;
		for (int j = 0; j < sizeof(alph); j++) {
			if (source[i] == alph[j]) {
				flag = true;
				float current_f = crypto_letters[source[i]];
				char min_diff_letter;
				float min_diff = 1.0;
				map <char, float>::iterator cur;
				int count = 0;
				for (cur = letters.begin(); cur != letters.end(); cur++){
					float current_diff = abs((*cur).second - current_f);
					if (current_diff < min_diff) {
						min_diff = current_diff;
						min_diff_letter = (*cur).first;
					}
				}
				decrypto_text += min_diff_letter;
			}
		}
		if (!flag) {
			decrypto_text += source[i];
		}
	}
	return decrypto_text;
}

string decrypting_bi(string source, map <string, float> letters, map <string, float> crypto_letters) {
	string decrypto_text, curr;
	for (int i = 0; i < source.length() - 1; i++) {
		bool flag = false;
		for (int j = 0; j < sizeof(alph); j++) {
			if (source[i] == alph[j]) {
				for (int c = 0; c < sizeof(alph); c++) {
					if (source[i + 1] == alph[c]) {
						flag = true;
						curr = source[i];
						curr += source[i + 1];
						float current_f = crypto_letters[curr];
						string min_diff_letters;
						float min_diff = 1.0;
						map <string, float>::iterator cur;
						int count = 0;
						string letters_mass = "";
						for (cur = letters.begin(); cur != letters.end(); cur++) {
							float current_diff = abs((*cur).second - current_f);
							if (current_diff < min_diff) {
								min_diff = current_diff;
								min_diff_letters = (*cur).first;
								letters_mass += min_diff_letters;
								letters_mass += " ";
							}
						}
						bool flag2 = true;
						for (int it = letters_mass.length() - 1; it > 0; it--) {
							if (letters_mass[it] != ' ') {
								string current_letters = "";
								current_letters += letters_mass[it - 1];
								current_letters += letters_mass[it];
								if (decrypto_text.length() != 0 && decrypto_text[decrypto_text.length() - 1] == current_letters[0]) {
									decrypto_text += current_letters[1];
									flag2 = false;
									it = 1;
								}
							}
						}
						if (flag2) {
							decrypto_text += min_diff_letters;
						}
						j, c = sizeof(alph);
					}
				}
			}
		}
		if (!flag) {
			bool flag3 = true;
			for (int j = 0; j < sizeof(alph); j++) {
				if (source[i] == alph[j]) {
					flag3 = false;
				}
			}
			if (flag3) {
				decrypto_text += source[i];
			}
		}
	}

	return decrypto_text;
}

string f_read (string source, string path) {
	ifstream f(path);
	string s;
	while (getline(f, s)) {
		source += s;
		source += "\n";
	}

	f.close();
	return source;
}

void f_write(string source, string path) {
	ofstream f;
	f.open(path);
	if (f.is_open()){
		f << source;
	}
}

int main()
{
	int replace = 15;

	map <string, float> letters;
	map <string, float> crypto_letters;
	
	string sup_source;	//for sample frequency
	string source;	//source text
	sup_source = f_read(sup_source, "C:/Users/Дом/source/repos/ConsoleApplication3/textSample.txt");
	source = f_read(source, "C:/Users/Дом/source/repos/ConsoleApplication3/textInput.txt");
	transform(sup_source.begin(), sup_source.end(), sup_source.begin(), tolower);
	transform(source.begin(), source.end(), source.begin(), tolower);
	string crypto_text;
	string decrypto_text;

	crypto_text = encrypting(source,replace);
	//letters = frequency_one(source, letters);
	letters = frequency_bi(source, letters);
	//crypto_letters = frequency_one(crypto_text, crypto_letters);
	crypto_letters = frequency_bi(crypto_text, crypto_letters);
	
	string str = "";
	for (auto t : letters) {
		str += t.first;
		str += " : ";
		str += to_string(t.second);
		str += "\n";
	}
	f_write(str, "C:/Users/Дом/source/repos/ConsoleApplication3/map.txt");

	//decrypto_text = decrypting_one(crypto_text, letters, crypto_letters);
	decrypto_text = decrypting_bi(crypto_text, letters, crypto_letters);
	
	f_write(crypto_text, "C:/Users/Дом/source/repos/ConsoleApplication3/textCrypto.txt");
	f_write(decrypto_text, "C:/Users/Дом/source/repos/ConsoleApplication3/textOutput.txt");
	return 0;
}