#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <string>

using namespace std;

int main() {
	setlocale(LC_ALL, "rus");
	string command;
	cout << "command: " << endl;
	cin >> command;
	if (command == "1") {
		FILE* picture; // начальная картинка
		unsigned long long len_of_picture = 0;
		FILE* text;  // текст, который надо скрыть
		unsigned long long len_of_text = 0;
		int count = 0;
		FILE* result_picture;  // результат (bmp)
		// пути к файлам 
		string path_to_picture, path_to_text;
		cout << "Введите путь к файлу (картинка):\n";
		cin >> path_to_picture;
		//path_to_picture = "1.bmp";
		cout << "Введите путь к файлу (текст):\n";
		cin >> path_to_text;
		//path_to_text = "1.txt";

		picture = fopen(path_to_picture.c_str(), "rb");
		text = fopen(path_to_text.c_str(), "r");

		while (!feof(text)) {
			char a = getc(text);
			len_of_text++;
		}
		fclose(text);
		result_picture = fopen("res.bmp", "wb");
		text = fopen(path_to_text.c_str(), "r");

		fseek(picture, 0, SEEK_END);
		len_of_picture = ftell(picture);
		fseek(picture, 0, SEEK_SET);

		count = len_of_picture;
		bool buf[8];
		unsigned char cur_pic;
		// 54 байта - заголовок файла
		for (int i = 0; i < 54; i++) {
			cur_pic = fgetc(picture);
			fputc(cur_pic, result_picture);
			count--;
		}

		// запись текста
		int cur_text = 0;
		bool sym[8];
		while (!feof(text)) {
			cur_text = fgetc(text);
			for (int i = 0; i < 8; i++) {
				sym[i] = cur_text % 2;
				cur_text /= 2;
			}
			for (int i = 0; i < 8; i++) {
				cur_pic = fgetc(picture);
				cur_pic = 0;
				for (int j = 0; j < 8; j++, i++)
					buf[j] = sym[i];
				for (int j = 0; j < 8; j++)
					cur_pic += buf[j] * (1 << j);
				fputc(cur_pic, result_picture);
				count--;
			}
		}
		while (count) {
			cur_pic = fgetc(picture);
			fputc(cur_pic, result_picture);
			count--;
		}
		fclose(picture);
		fclose(text);
		fclose(result_picture);
	}
	else if (command == "2") {
		FILE* picture; // картинка с текстом
		unsigned long long len_of_picture = 0;
		string text; // спрятанный текст

		int count;

		string path_to_picture;
		cout << "Path (picture):" << endl;
		cin >> path_to_picture;
		//path_to_picture = "res.bmp";

		picture = fopen(path_to_picture.c_str(), "rb");
		fseek(picture, 0, SEEK_END);
		len_of_picture = ftell(picture);
		fseek(picture, 0, SEEK_SET);

		count = len_of_picture;
		bool buf[8], sym[8];
		unsigned char cur_pic;

		for (int i = 0; i < 54; i++) {
			cur_pic = fgetc(picture);
			count--;
		}
		for (int i = 54; i < 93; i++) {
			cur_pic = fgetc(picture);
			text += cur_pic;
		}
		FILE* result_text;
		result_text = fopen("result text.txt", "w");
		fputs(text.c_str(), result_text);
		cout << text;
	}

	return 0;
}