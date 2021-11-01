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
	cout << "Введите путь к файлу (картинка):\n";
	cin >> path_to_picture;
	path_to_picture = "1.bmp";
	cout << "Введите путь к файлу (текст):\n";
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
	// 38 байта - заголовок файла
	for (int i = 0; i < 38; i++) {
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
	cout << "Path (picture):" << endl;
	cin >> path_to_picture;
	path_to_picture = "res.bmp";

	picture = fopen(path_to_picture.c_str(), "rb");
	fseek(picture, 0, SEEK_END);
	len_of_picture = ftell(picture);
	fseek(picture, 0, SEEK_SET);

	bool buf[8], sym[8];
	unsigned char cur_pic;

	for (int i = 0; i < 38; i++)
		cur_pic = fgetc(picture);
	for (int i = 38; i < 120; i++) {
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
	cout << "Path (original): " << endl;
	cin >> path1;
	path1 = "1.bmp";
	cout << "Path (checking for hidden text): " << endl;
	cin >> path2;
	path2 = "res.bmp";
	picture1 = fopen(path1.c_str(), "rb");
	picture2 = fopen(path2.c_str(), "rb");

	unsigned char cur_pic;
	for (int i = 0; i < 38; i++) {
		cur_pic = fgetc(picture1);
		cur_pic = fgetc(picture2);
	}
	for (int i = 38; i < 120; i++) {
		cur_pic = fgetc(picture1);
		text1 += cur_pic;
		cur_pic = fgetc(picture2);
		text2 += cur_pic;
	}

	if (text1 == text2)
		cout << "The files are the same";
	else {
		string hidden;
		for (int i = 0; i < text1.length(); i++)
			if (text1[i] != text2[i])
				hidden += text2[i];
		cout << "In file2: " << endl;
		cout << hidden;
	}
}

int main() {
	start();
	string command;
	cin >> command;
	if (command == "1")
		hide();
	else if (command == "2")
		show();
	else if (command == "3")
		compare();
	else
		cout << "Такой команды нет" << endl;

	return 0;
}