#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <string>

using namespace std;

void start() {
	setlocale(LC_ALL, "rus");
	cout << "МЕНЮ\n1 - Спрятать текст\n2 - Посмотреть содержимое\n3 - Сравнить с оригиналом\n\nВведите номер команды:\n";
}
void hide() {
	FILE* picture; // начальная картинка
	unsigned long long len_of_picture = 0;
	FILE* text;  // текст, который надо скрыть
	unsigned long long len_of_text = 0;
	int count = 0;
	FILE* result_picture;  // результат (bmp)

	// пути к файлам 
	string path_to_picture, path_to_text;
	cout << "Введите название файла (картинка):\n";
	cin >> path_to_picture;
	path_to_picture = "1.bmp";
	cout << "Введите название файла (текст):\n";
	cin >> path_to_text;
	path_to_text = "1.txt";

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
	// 14 байт - заголовок файла + 20 - заголовок растровой информации (важная инфа для запуска) (max - 40)
	// (6 - 9 биты зарезервированы -> их тоже можно использовать, но длинна входного текста очень мала)
	// все числа в 16 сс
	for (int i = 0; i < 14 + 20; i++) {
		cur_pic = fgetc(picture);
		fputc(cur_pic, result_picture);
		count--;
	}

	// запись текста
	int cur_text = 0;
	bool sym[8];
	while (len_of_text - 1) {
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
		len_of_text--;
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
void show() {
	FILE* picture; // картинка с текстом
	unsigned long long len_of_picture = 0;
	string text; // спрятанный текст

	string path_to_picture;
	cout << "Введите название файла (картинка):" << endl;
	cin >> path_to_picture;
	path_to_picture = "res.bmp";

	picture = fopen(path_to_picture.c_str(), "rb");
	fseek(picture, 0, SEEK_END);
	len_of_picture = ftell(picture);
	fseek(picture, 0, SEEK_SET);

	bool buf[8], sym[8];
	unsigned char cur_pic;

	for (int i = 0; i < 14; i++)
		cur_pic = fgetc(picture);
	for (int i = 14; i < 120; i++) {
		cur_pic = fgetc(picture);
		text += cur_pic;
	}
	FILE* result_text;
	result_text = fopen("result text.txt", "w");
	fputs(text.c_str(), result_text);
	cout << text;
	/*FILE* result;
	result = fopen("encript text.txt", "w");
	fputs(text.c_str(), result);
	fclose(result);*/
}
void compare() {
	// original
	FILE* picture1;
	string text1;
	// check
	FILE* picture2;
	string text2;

	string path1, path2;
	cout << "Введите название файла 1: " << endl;
	cin >> path1;
	path1 = "1.bmp";
	cout << "Введите название файла 2: " << endl;
	cin >> path2;
	path2 = "res.bmp";
	picture1 = fopen(path1.c_str(), "rb");
	picture2 = fopen(path2.c_str(), "rb");

	unsigned char cur_pic;
	for (int i = 0; i < 14; i++) {
		cur_pic = fgetc(picture1);
		cur_pic = fgetc(picture2);
	}
	for (int i = 14; i < 120; i++) {
		cur_pic = fgetc(picture1);
		text1 += cur_pic;
		cur_pic = fgetc(picture2);
		text2 += cur_pic;
	}

	if (text1 == text2)
		cout << "Файлы одинаковые";
	else {
		string hidden;
		for (int i = 0; i < text1.length(); i++)
			if (text1[i] != text2[i])
				hidden += text2[i];
		cout << "В файле 2 спрятан текст: " << endl;
		cout << hidden;
	}
}
void lsb() {
	FILE* picture; // начальная картинка
	unsigned long long len_of_picture = 0;
	FILE* text;  // текст, который надо скрыть
	unsigned long long len_of_text = 0;
	int count = 0;
	FILE* result_picture;  // результат (bmp)
	string text_str;

	// пути к файлам 
	string path_to_picture, path_to_text;
	cout << "Введите название файла (картинка):\n";
	cin >> path_to_picture;
	path_to_picture = "1.bmp";
	cout << "Введите название файла (текст):\n";
	cin >> path_to_text;
	path_to_text = "1.txt";

	picture = fopen(path_to_picture.c_str(), "rb");
	text = fopen(path_to_text.c_str(), "r");

	while (!feof(text)) {
		char a = getc(text);
		text_str += a;
		len_of_text++;
	}
	text_str.erase(text_str.end() - 1);
	len_of_text--;
	fclose(text);
	result_picture = fopen("res.bmp", "wb");
	text = fopen(path_to_text.c_str(), "r");

	fseek(picture, 0, SEEK_END);
	len_of_picture = ftell(picture);
	fseek(picture, 0, SEEK_SET);
	count = len_of_picture;

	if (len_of_text * 8 > len_of_picture - 84) {
		cout << "Файл с текстом слишком большой";
		return;
	}

	unsigned char cur_pic;
	for (int i = 0; i < 54; i++) {
		cur_pic = fgetc(picture);
		fputc(cur_pic, result_picture);
		count--;
	}

	while (len_of_text) {
		unsigned char letter = fgetc(text);
		bool sym[8];
		for (int i = 0; i < 8; i++) {
			sym[i] = letter % 2;
			letter /= 2;
		}
		for (int i = 0; i < 8; i++) {
			cur_pic = fgetc(picture);
			bool buf[8];
			for (int j = 0; j < 8; j++) {
				buf[j] = cur_pic % 2;
				cur_pic /= 2;
			}
			buf[7] = sym[i];
			cur_pic = 0;
			for (int j = 0; j < 8; j++)
				cur_pic += buf[j] * (1 << j);
			fputc(cur_pic, result_picture);
			count--;
		}
		len_of_text--;
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
void lsb_show() {
	FILE* picture; // начальная картинка
	unsigned long long len_of_picture = 0;
	unsigned long long len_of_text = 0;
	int count = 0;
	FILE* result_picture;  // результат (bmp)
	string text_str;

	// пути к файлам 
	string path_to_picture, path_to_text;
	cout << "Введите название файла (картинка):\n";
	cin >> path_to_picture;
	path_to_picture = "res.bmp";

	cout << "Введите длину скрытого текста. Если не знаете - введите 0:\n";
	cin >> len_of_text;

	picture = fopen(path_to_picture.c_str(), "rb");

	fseek(picture, 0, SEEK_END);
	len_of_picture = ftell(picture);
	fseek(picture, 0, SEEK_SET);
	count = len_of_picture;

	unsigned char cur_pic;
	for (int i = 0; i < 54; i++) {
		cur_pic = fgetc(picture);
		count--;
	}

	string result;
	if (len_of_text == 0) {
		bool buf[8];
		int k = 0;

		while (count) {
			if (k % 8 == 0) {
				unsigned char sym = 0;
				for (int i = 0; i < 8; i++) {
					sym += buf[i] * (1 << i);
				}
				k = 0;
				result += sym;
			}
			cur_pic = fgetc(picture);
			bool tmp_buf[8];
			for (int i = 0; i < 8; i++) {
				tmp_buf[i] = cur_pic % 2;
				cur_pic /= 2;
			}
			buf[k] = tmp_buf[7];
			k++;
			count--;
		}
	}
	else {
		bool buf[8] = {};
		int k = 0;

		for (int i = 0; i < len_of_text * 8; i++) {
			if (k % 8 == 0) {
				unsigned char sym = 0;
				for (int j = 0; j < 8; j++) 
					sym += buf[j] * (1 << j);
				k = 0;
				result += sym;
			}
			cur_pic = fgetc(picture);
			bool tmp_buf[8] = {};
			for (int j = 0; j < 8; j++) {
				tmp_buf[j] = cur_pic % 2;
				cur_pic /= 2;
			}
			buf[k] = tmp_buf[7];
			k++;
		}
		unsigned char sym = 0;
		for (int j = 0; j < 8; j++) 
			sym += buf[j] * (1 << j);
		result += sym;
	}

	cout << result.length() << " - " << result;
}

int main() {
	start();
	string command;
	cin >> command;
	if (command == "1")
		lsb();
	//hide();
	else if (command == "2")
		lsb_show();
		//show();
	else if (command == "3")
		compare();
	else
		cout << "Такой команды нет" << endl;

	return 0;
}