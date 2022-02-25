#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <string>

using namespace std;

void start() {
	/*Эта функция выводит меню*/
	setlocale(LC_ALL, "rus");
	cout << "МЕНЮ\n1 - Спрятать текст\n2 - Посмотреть содержимое\n3 - Сравнить с оригиналом\n\nВведите номер команды:\n";
}
void hide_in_title() {
	/*Эта функция прячет текст в заголовок файла*/
	FILE* picture;								// начальная картинка
	unsigned long long len_of_picture = 0;		// количество байтов в картинке
	FILE* text;									// текст, который надо скрыть (файл)
	unsigned long long len_of_text = 0;			// длина текста
	int count = 0;								// счётчик, отвечающий за количество оставшихся пикселей
	FILE* result_picture;						// результат (bmp)

	// ввод файлов
	string path_to_picture, path_to_text;
	cout << "Введите название файла (картинка):\n";
	cin >> path_to_picture;
	path_to_picture = "1.bmp";
	cout << "Введите название файла (текст):\n";
	cin >> path_to_text;
	path_to_text = "1.txt";

	// определение длины текста
	text = fopen(path_to_text.c_str(), "r");
	while (!feof(text)) {
		char a = getc(text);
		len_of_text++;
	}
	fclose(text);

	// создание картинки-результата
	result_picture = fopen("res.bmp", "wb");

	// определение количества пикселей в картинке
	picture = fopen(path_to_picture.c_str(), "rb");
	fseek(picture, 0, SEEK_END);
	len_of_picture = ftell(picture);
	fseek(picture, 0, SEEK_SET);
	count = len_of_picture;

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
	text = fopen(path_to_text.c_str(), "r");
	int cur_text = 0;
	bool sym[8];
	bool buf[8];
	while (len_of_text - 1) {
		cur_text = fgetc(text);
		// перевод байта с буквой в 8 бит
		for (int i = 0; i < 8; i++) {
			sym[i] = cur_text % 2;
			cur_text /= 2;
		}
		// замена байта
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

	// добавление оставшихся байт
	while (count) {
		cur_pic = fgetc(picture);
		fputc(cur_pic, result_picture);
		count--;
	}

	fclose(picture);
	fclose(text);
	fclose(result_picture);
}
void show_title() {
	/*Эта функция выводит заголовок файла*/
	FILE* picture;								// картинка с текстом
	unsigned long long len_of_picture = 0;		// количество байт в картинке
	string text;								// спрятанный текст

	// ввод файла
	string path_to_picture;
	cout << "Введите название файла (картинка):" << endl;
	cin >> path_to_picture;
	path_to_picture = "res.bmp";

	// определение количества байт в картинке
	picture = fopen(path_to_picture.c_str(), "rb");
	fseek(picture, 0, SEEK_END);
	len_of_picture = ftell(picture);
	fseek(picture, 0, SEEK_SET);

	bool buf[8], sym[8];
	unsigned char cur_pic;

	// пропуск первых 14 байт
	for (int i = 0; i < 14; i++)
		cur_pic = fgetc(picture);

	// вывод содержимого
	for (int i = 14; i < 120; i++) {
		cur_pic = fgetc(picture);
		text += cur_pic;
	}
	cout << text;
}
void compare() {
	/*Эта функция сравнивает 2 файла и выводит их отличия*/
	FILE* picture1;				// файл-оригинал
	string text1;				// его содержимое
	FILE* picture2;				// файл, который надо проверить
	string text2;				// его содержимое

	// ввод файлов
	string path1, path2;
	cout << "Введите название файла 1: " << endl;
	cin >> path1;
	path1 = "1.bmp";
	cout << "Введите название файла 2: " << endl;
	cin >> path2;
	path2 = "res.bmp";
	picture1 = fopen(path1.c_str(), "rb");
	picture2 = fopen(path2.c_str(), "rb");

	// пропуск заголовка
	unsigned char cur_pic;
	for (int i = 0; i < 14; i++) {
		cur_pic = fgetc(picture1);
		cur_pic = fgetc(picture2);
	}

	// составление содержимого файлов
	for (int i = 14; i < 120; i++) {
		cur_pic = fgetc(picture1);
		text1 += cur_pic;
		cur_pic = fgetc(picture2);
		text2 += cur_pic;
	}

	// сравнивние
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
	/*Метод наименее значимых битов*/
	FILE* picture;								// начальная картинка
	unsigned long long len_of_picture = 0;		// количество байт в картинке
	FILE* text;									// текст, который надо скрыть (файл)
	string text_str;							// его содержимое
	unsigned long long len_of_text = 0;			// длина текста
	int count = 0;								// счётчик, отвечающий за оставшиеся байты
	FILE* result_picture;						// результат (bmp)

	// пути к файлам 
	string path_to_picture, path_to_text;
	cout << "Введите название файла (картинка):\n";
	cin >> path_to_picture;
	path_to_picture = "1.bmp";
	cout << "Введите название файла (текст):\n";
	cin >> path_to_text;
	path_to_text = "1.txt";

	// определение длины текста
	text = fopen(path_to_text.c_str(), "r");
	while (!feof(text)) {
		char a = getc(text);
		text_str += a;
		len_of_text++;
	}
	// удаление последнего символа (возврата каретки)
	text_str.erase(text_str.end() - 1);
	len_of_text--;
	fclose(text);

	// создание картинки-результата
	result_picture = fopen("res.bmp", "wb");

	// определение количеста пикселей в картинке
	picture = fopen(path_to_picture.c_str(), "rb");
	fseek(picture, 0, SEEK_END);
	len_of_picture = ftell(picture);
	fseek(picture, 0, SEEK_SET);
	count = len_of_picture;

	// проверка на то, что текст поместится в картинку
	if (len_of_text * 8 > len_of_picture - 84) {
		cout << "Файл с текстом слишком большой";
		return;
	}

	// пропуск заголовка файла
	unsigned char cur_pic;
	for (int i = 0; i < 110; i++) {
		cur_pic = fgetc(picture);
		fputc(cur_pic, result_picture);
		count--;
	}

	// запись текста
	text = fopen(path_to_text.c_str(), "r");
	while (len_of_text) {
		unsigned char letter = fgetc(text);
		bool sym[8];
		// перевод буквы в формат 8-ми битов
		for (int i = 0; i < 8; i++) {
			sym[i] = letter % 2;
			letter /= 2;
		}
		for (int i = 0; i < 8; i++) {
			cur_pic = fgetc(picture);
			bool buf[8];
			// перевод пикселя в 8-ми битный формат
			for (int j = 0; j < 8; j++) {
				buf[j] = cur_pic % 2;
				cur_pic /= 2;
			}
			// изменение последнего бита
			buf[7] = sym[i];
			// запись изменённого бита
			cur_pic = 0;
			for (int j = 0; j < 8; j++)
				cur_pic += buf[j] * (1 << j);
			fputc(cur_pic, result_picture);
			count--;
		}
		len_of_text--;
	}

	// запись оставшихся битов
	while (count) {
		cur_pic = fgetc(picture);
		fputc(cur_pic, result_picture);
		count--;
	}

	fclose(picture);
	fclose(text);
	fclose(result_picture);
}
void show_lsb() {
	/*Эта функция выводит текст, скрытый методом lsb*/
	FILE* picture;								// начальная картинка
	unsigned long long len_of_picture = 0;		// количество байт в картинке
	unsigned long long len_of_text = 0;			// длина текста
	int count = 0;								// счётчик, отвечающий за оставшиеся байты
	FILE* result_picture;						// картинка, в которую встроен текст
	string text_str;							// результат

	// пути к файлам 
	string path_to_picture, path_to_text;
	cout << "Введите название файла (картинка):\n";
	cin >> path_to_picture;
	path_to_picture = "res.bmp";
	picture = fopen(path_to_picture.c_str(), "rb");

	cout << "Введите длину скрытого текста. Если не знаете - введите 0:\n";
	cin >> len_of_text;

	// определение количества пикселей картинки
	fseek(picture, 0, SEEK_END);
	len_of_picture = ftell(picture);
	fseek(picture, 0, SEEK_SET);
	count = len_of_picture;

	// пропуск заголовка
	unsigned char cur_pic;
	for (int i = 0; i < 110; i++) {
		cur_pic = fgetc(picture);
		count--;
	}

	// получение текста
	string result;
	if (len_of_text == 0) {
		/*Если пользователь не знает длину текста, то выведется весь файл*/
		bool buf[8];
		int k = 0;
		while (count) {
			// если буфер заполнен, то преобразуем его в символ
			if (k % 8 == 0) {
				unsigned char sym = 0;
				for (int i = 0; i < 8; i++) {
					sym += buf[i] * (1 << i);
				}
				k = 0;
				result += sym;
			}
			// берём пиксель
			cur_pic = fgetc(picture);
			// преобразуем его в 8-ми битный формат
			bool tmp_buf[8];
			for (int i = 0; i < 8; i++) {
				tmp_buf[i] = cur_pic % 2;
				cur_pic /= 2;
			}
			// берём последний бит
			buf[k] = tmp_buf[7];
			k++;
			count--;
		}
	}
	else {
		/*Если пользователь знает длину текста, то выведутся только первые n байт*/
		bool buf[8] = {};
		int k = 0;
		for (int i = 0; i < len_of_text * 8; i++) {
			// если буфер заполнен, то преобразуем его в символ
			if (k % 8 == 0) {
				unsigned char sym = 0;
				for (int j = 0; j < 8; j++) 
					sym += buf[j] * (1 << j);
				k = 0;
				result += sym;
			}
			// берём пиксель
			cur_pic = fgetc(picture);
			// преобразуем его в 8-ми битный формат
			bool tmp_buf[8] = {};
			for (int j = 0; j < 8; j++) {
				tmp_buf[j] = cur_pic % 2;
				cur_pic /= 2;
			}
			// берём последний бит
			buf[k] = tmp_buf[7];
			k++;
		}
		unsigned char sym = 0;
		for (int j = 0; j < 8; j++) 
			sym += buf[j] * (1 << j);
		result += sym;
	}

	cout << result;
}

int main() {
	start();
	string command;
	cin >> command;
	if (command == "1") {
		cout << "1 - Метод встраивания в заголовок\n2 - Метод LSB\n\nВведите номер команды:\n";
		cin >> command;
		if (command == "1")
			hide_in_title();
		else if (command == "2")
			lsb();
		else
			cout << "Такой команды нет" << endl;
	}
	else if (command == "2") {
		cout << "1 - Методом встраивания в заголовок\n2 - Методом LSB\n\nВведите номер команды:\n";
		cin >> command;
		if (command == "1")
			show_title();
		else if (command == "2")
			show_lsb();
		else
			cout << "Такой команды нет" << endl;
	}
	else if (command == "3")
		compare();
	else
		cout << "Такой команды нет" << endl;

	return 0;
}