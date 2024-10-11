#include "pch.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

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
		len_of_picture = 0;
		len_of_text = 0;
		count = 0;
		text_str = "";
	}
	/*~Model() {
		fclose(picture);
		fclose(text);
		fclose(result_picture);
		fclose(picture1);
		fclose(picture2);
	}*/

	void calculate_len_of_text(string path_to_text) {
		// определение длины текста
		text = fopen(path_to_text.c_str(), "r");
		while (!feof(text)) {
			char a = getc(text);
			len_of_text++;
		}
		fclose(text);
	}

	void create_result_picture() {
		// создание картинки-результата
		result_picture = fopen("res.bmp", "wb");
	}

	void calculate_number_of_pixels(string path_to_picture) {
		// определение количества пикселей в картинке
		picture = fopen(path_to_picture.c_str(), "rb");
		fseek(picture, 0, SEEK_END);
		len_of_picture = ftell(picture);
		fseek(picture, 0, SEEK_SET);
		count = len_of_picture;
	}

	void hide_in_title(string path_to_picture, string path_to_text) {
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
		// определение количества байт в картинке
		picture = fopen(path_to_picture.c_str(), "rb");
		fseek(picture, 0, SEEK_END);
		len_of_picture = ftell(picture);
		fseek(picture, 0, SEEK_SET);
	}

	void show_title(string path_to_picture) {
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
		///*Эта функция сравнивает 2 файла и выводит их отличия*/
		//FILE* picture1;				// файл-оригинал
		//string text1;				// его содержимое
		//FILE* picture2;				// файл, который надо проверить
		//string text2;				// его содержимое

		//// ввод файлов
		//string path1, path2;
		//cout << "Введите название файла 1: " << endl;
		//cin >> path1;
		//path1 = "1.bmp";
		//cout << "Введите название файла 2: " << endl;
		//cin >> path2;
		//path2 = "res.bmp";
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
		len_of_text = text.length();
		int ind = 0;
		while (len_of_text) {
			unsigned char letter = text[ind];
			bool sym[8];
			// перевод буквы в формат 8-ми битов
			for (int i = 0; i < 8; i++) {
				sym[i] = letter % 2;
				letter /= 2;
			}
			for (int i = 0; i < 8; i++) {
				unsigned char cur_pic = arr[ind];
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
				arr[ind] = cur_pic;
			}
			ind++;
			len_of_text--;
		}
	}
};

//TEST(test1, lsb) {
//	const int n = 10;
//	int arr[n] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
//	string text = "a";
//	Model model;
//	model.test_lsb(arr, n, text);
//	for (int i = 0; i < n; i++)
//		cout << arr[i] << " ";
//	EXPECT_EQ(1, 1);
//}

TEST(test2, lsb) {
	Model model;
	string path_to_picture = "C:\\Users\\timat\\OneDrive\\Рабочий стол\\программирование\\c++\\steganography\\stego\\1.bmp";
	string path_to_text = "C:\\Users\\timat\\OneDrive\\Рабочий стол\\программирование\\c++\\steganography\\stego\\1.txt";
	string text;
	ifstream fin(path_to_text);
	fin >> text;
	fin.close();
	model.lsb(path_to_picture, path_to_text);

	ostringstream oss;
	streambuf* cout_streambuf = cout.rdbuf();
	cout.rdbuf(oss.rdbuf());

	string path_to_result = "C:\\Users\\timat\\OneDrive\\Рабочий стол\\программирование\\c++\\steganography\\x64\\Debug\\res.bmp";
	model.compare(path_to_picture, path_to_result);

	cout.rdbuf(cout_streambuf);
	EXPECT_NE(oss.str(), "Файлы одинаковые");
}

//TEST(test1, hide_in_title) {
//	EXPECT_EQ(1, 1);
//}

TEST(test2, hide_in_title) {
	Model model;
	string path_to_picture = "C:\\Users\\timat\\OneDrive\\Рабочий стол\\программирование\\c++\\steganography\\stego\\1.bmp";
	string path_to_text = "C:\\Users\\timat\\OneDrive\\Рабочий стол\\программирование\\c++\\steganography\\stego\\1.txt";
	string text;
	ifstream fin(path_to_text);
	fin >> text;
	fin.close();
	model.hide_in_title(path_to_picture, path_to_text);

	ostringstream oss;
	streambuf* cout_streambuf = cout.rdbuf();
	cout.rdbuf(oss.rdbuf());

	string path_to_result = "C:\\Users\\timat\\OneDrive\\Рабочий стол\\программирование\\c++\\steganography\\x64\\Debug\\res.bmp";
	model.compare(path_to_picture, path_to_result);

	cout.rdbuf(cout_streambuf);
	EXPECT_EQ(oss.str(), "В файле 2 спрятан текст: \n" + text);
}
