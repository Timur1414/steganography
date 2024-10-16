﻿#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <string>

using namespace std;


class Model {
private:
	FILE* picture;						// начальная картинка
	unsigned long long len_of_picture;	// количество байтов в картинке
	FILE* text;							// текст, который надо скрыть (файл)
	unsigned long long len_of_text;		// длина текста
	int count;							// счётчик, отвечающий за количество оставшихся пикселей
	FILE* result_picture;				// результат (bmp)
	string text_str;					// спрятанный текст

	FILE* picture1;						// файл-оригинал
	string text1;						// его содержимое
	FILE* picture2;						// файл, который надо проверить
	string text2;						// его содержимое

public:
	Model() {
		// конструктор класса
		len_of_picture = 0;
		len_of_text = 0;
		count = 0;
		text_str = "";
	}

	void calculate_len_of_text(string path_to_text) {
		// метод определения длины текста
		// param: path_to_text путь к файлу с текстом
		text = fopen(path_to_text.c_str(), "r");
		while (!feof(text)) {
			char a = getc(text);
			len_of_text++;
		}
		fclose(text);
	}

	void create_result_picture() {
		// метод создания картинки-результата
		result_picture = fopen("res.bmp", "wb");
	}

	void calculate_number_of_pixels(string path_to_picture) {
		// метод определения количества пикселей в картинке
		// param: path_to_picture путь к изображению
		picture = fopen(path_to_picture.c_str(), "rb");
		fseek(picture, 0, SEEK_END);
		len_of_picture = ftell(picture);
		fseek(picture, 0, SEEK_SET);
		count = len_of_picture;
	}

	void hide_in_title(string path_to_picture, string path_to_text) {
		// метод алгоритма встраивания в заголовок
		// param: path_to_picture путь к изображению
		// param: path_to_text путь к файлу с текстом
		calculate_len_of_text(path_to_text);

		create_result_picture();

		calculate_number_of_pixels(path_to_picture);

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

	void calculate_number_of_bytes(string path_to_picture) {
		// метод определения количества байт в картинке
		// param: path_to_picture путь к изображению
		picture = fopen(path_to_picture.c_str(), "rb");
		fseek(picture, 0, SEEK_END);
		len_of_picture = ftell(picture);
		fseek(picture, 0, SEEK_SET);
	}

	void show_title(string path_to_picture) {
		// метод вывода заголовка файла
		// param: path_to_picture путь к изображению
		calculate_number_of_bytes(path_to_picture);

		bool buf[8], sym[8];
		unsigned char cur_pic;

		// пропуск первых 14 байт
		for (int i = 0; i < 14; i++)
			cur_pic = fgetc(picture);

		// вывод содержимого
		for (int i = 14; i < 120; i++) {
			cur_pic = fgetc(picture);
			text_str += cur_pic;
		}
		cout << text_str;
	}

	void compare(string path1, string path2) {
		// метод сравнивания двух файлов. Этот метод также выводит отличия
		// param: path1 путь к первому изображению
		// param: path2 путь к второму изображению
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

	void lsb(string path_to_picture, string path_to_text) {
		// метод алгоритма lsb
		// param: path_to_picture путь к изображению
		// param: path_to_text путь к файлу с текстом
		calculate_len_of_text(path_to_text);

		create_result_picture();

		calculate_number_of_pixels(path_to_picture);

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

	void show_lsb(string path_to_picture, string path_to_text, int len_of_text) {
		// метод нахождения текста по алгоритму lsb
		// param: path_to_picture путь к изображению
		// param: path_to_text путь к файлу с текстом
		// param: len_of_text количество символов в тексте
		calculate_number_of_pixels(path_to_picture);

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

	void test_lsb(int* arr, const int n, string text) {
		// метод для тестирования алгоритма lsb
		// param: arr массив чисел (байтов)
		// param: n размер массива
		// param: text текст, который надо спрятать
		len_of_text = text.length();
		int i = 0;
		while (len_of_text) {
			unsigned char letter = text[i];
			bool sym[8];
			// перевод буквы в формат 8-ми битов
			for (int i = 0; i < 8; i++) {
				sym[i] = letter % 2;
				letter /= 2;
			}
			for (int i = 0; i < 8; i++) {
				unsigned char cur_pic = arr[i];
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
				arr[i] = cur_pic;
			}
			i++;
			len_of_text--;
		}
	}
};


class Controller {
private:
	Model* model;						// указатель на модель
	int command1;						// первый выбор команды
	int command2;						// второй выбор команды
	string method;						// итоговый метод работы кода
public:
	Controller(Model* model) {
		// конструктор класса
		// param: model указатель на модель
		this->model = model;
	}

	string get_method() {
		// get-тер для метода работы кода
		return method;
	}

	int first_input(string cmd) {
		// метод проверки и сохранения команды введённой на первом этапе
		// param: cmd строка, которую ввёл пользователь
		try {
			command1 = stoi(cmd);
			if (command1 < 0 || command1 > 3)
				throw logic_error("1 - 3");
		}
		catch (...) {
			command1 = -1;
		}
		return command1;
	}

	void second_input(string cmd) {
		// метод проверки и сохранения команды введённой на втором этапе
		// param: cmd строка, которую ввёл пользователь
		try {
			command2 = stoi(cmd);
			if (command2 < 0 || command2 > 2)
				throw logic_error("1 - 2");
		}
		catch (...) {
			command2 = -1;
		}
		select_method();
	}

	void select_method() {
		// метод определения метода работы кода
		if (command1 == 1) {
			if (command2 == 1)
				method = "hide_in_title";
			else
				method = "lsb";
		}
		else if (command1 == 2) {
			if (command2 == 1)
				method = "show_title";
			else
				method = "show_lsb";
		}
		else
			method = "compare";
	}

	void execute(string path1, string path2) {
		// метод вызова нужного метода модели
		// param: path1 путь до первого файла
		// param: path2 путь до второго файла
		if (method == "hide_in_title")
			model->hide_in_title(path1, path2);
		else if (method == "lsb")
			model->lsb(path1, path2);
		else
			model->compare(path1, path2);
	}

	void execute(string path_to_picture) {
		// метод вызова нужного метода модели
		// param: path_to_picture путь до изображения
		model->show_title(path_to_picture);
	}

	void execute(string path_to_picture, string path_to_text, string len_of_text) {
		// метод вызова нужного метода модели
		// param: path_to_picture путь до изображения
		// param: path_to_text путь до файла с текстом
		// param: len_of_text длина текста
		model->show_lsb(path_to_picture, path_to_text, stoi(len_of_text));
	}
};


class View {
private:
	Controller* controller;						// указатель на контроллер
public:
	View(Controller* controller) {
		// конструктор класса
		// param: controller указатель на контроллер
		this->controller = controller;
	}

	void prepare() {
		// метод установки русского языка
		setlocale(LC_ALL, "rus");
	}

	void start() {
		// метод вывода первого меню
		string cmd;
		cout << "МЕНЮ\n1 - Спрятать текст\n2 - Посмотреть содержимое\n3 - Сравнить с оригиналом\n\nВведите номер команды:\n";
		cin >> cmd;
		int next_message = controller->first_input(cmd);
		if (next_message == -1)
			error_message();
		else if (next_message == 1)
			show_menu1();
		else if (next_message == 2)
			show_menu2();
		else
			show_menu3();
		enter_data();
	}

	void enter_data() {
		// метод считывания данных
		string method = controller->get_method();
		if (method == "hide_in_title" || method == "lsb")
			enter_files();
		else if (method == "show_title")
			enter_file();
		else if (method == "show_lsb")
			enter_file_with_length();
		else
			enter_two_pictures();
	}

	void enter_two_pictures() {
		// метод ввода данных
		string path1, path2;
		cout << "Введите название файла 1: " << endl;
		cin >> path1;
		path1 = "1.bmp";
		cout << "Введите название файла 2: " << endl;
		cin >> path2;
		path2 = "res.bmp";
		controller->execute(path1, path2);
	}

	void enter_file() {
		// метод ввода данных
		string path_to_picture;
		cout << "Введите название файла (картинка):" << endl;
		cin >> path_to_picture;
		path_to_picture = "res.bmp";
		controller->execute(path_to_picture);
	}

	void enter_file_with_length() {
		// метод ввода данных 
		string path_to_picture, path_to_text, len_of_text;
		cout << "Введите название файла (картинка):\n";
		cin >> path_to_picture;
		path_to_picture = "res.bmp";
		cout << "Введите длину скрытого текста. Если не знаете - введите 0:\n";
		cin >> len_of_text;
		controller->execute(path_to_picture, path_to_text, len_of_text);
	}

	void enter_files() {
		// метод ввода данных
		string path_to_picture, path_to_text;
		cout << "Введите название файла (картинка):\n";
		cin >> path_to_picture;
		path_to_picture = "1.bmp";
		cout << "Введите название файла (текст):\n";
		cin >> path_to_text;
		path_to_text = "1.txt";
		controller->execute(path_to_picture, path_to_text);
	}

	void error_message() {
		// метод вывода ошибки
		cout << "Такой команды нет" << endl;
	}

	void show_menu1() {
		// метод вывода второго меню
		string cmd;
		cout << "1 - Метод встраивания в заголовок\n2 - Метод LSB\n\nВведите номер команды:\n";
		cin >> cmd;
		controller->second_input(cmd);
	}

	void show_menu2() {
		// метод вывода второго меню
		string cmd;
		cout << "1 - Методом встраивания в заголовок\n2 - Методом LSB\n\nВведите номер команды:\n";
		cin >> cmd;
		controller->second_input(cmd);
	}

	void show_menu3() {
		// метод вывода второго меню
		controller->second_input("");
	}
};


int main() {
	Model model;
	Controller controller(&model);
	View view(&controller);
	view.prepare();
	view.start();
	

	return 0;
}