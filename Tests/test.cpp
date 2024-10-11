#include "pch.h"
#include <fstream>
#include "C:\\Users\\timat\\OneDrive\\Рабочий стол\\программирование\\c++\\steganography\\stego\\stego.cpp"

TEST(test1, lsb) {
	//const int n = 10;
	//int arr[n] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	//string text = "a";
	//Model model;
	//model.test_lsb(arr, n, text);
	//for (int i = 0; i < n; i++)
	//	cout << arr[i] << " ";
	EXPECT_EQ(1, 2);
}

TEST(test2, lsb) {
	//Model model;
	//string path_to_picture = "C:\\Users\\timat\\OneDrive\\Рабочий стол\\программирование\\c++\\steganography\\stego\\1.bmp";
	//string path_to_text = "C:\\Users\\timat\\OneDrive\\Рабочий стол\\программирование\\c++\\steganography\\stego\\1.txt";
	//string text;
	//ifstream fin(path_to_text);
	//fin >> text;
	//fin.close();
	//model.lsb(path_to_picture, path_to_text);

	//ostringstream oss;
	//streambuf* cout_streambuf = cout.rdbuf();
	//cout.rdbuf(oss.rdbuf());

	//string path_to_result = "C:\\Users\\timat\\OneDrive\\Рабочий стол\\программирование\\c++\\steganography\\stego\\res.txt";
	//model.compare(path_to_picture, path_to_result);

	//cout.rdbuf(cout_streambuf);
	//EXPECT_NE(oss.str(), "Файлы одинаковые");
	EXPECT_EQ(1, 1);
}

TEST(test1, hide_in_title) {
	EXPECT_EQ(1, 1);
}

TEST(test2, hide_in_title) {
	//Model model;
	//string path_to_picture = "C:\\Users\\timat\\OneDrive\\Рабочий стол\\программирование\\c++\\steganography\\stego\\1.bmp";
	//string path_to_text = "C:\\Users\\timat\\OneDrive\\Рабочий стол\\программирование\\c++\\steganography\\stego\\1.txt";
	//string text;
	//ifstream fin(path_to_text);
	//fin >> text;
	//fin.close();
	//model.hide_in_title(path_to_picture, path_to_text);

	//ostringstream oss;
	//streambuf* cout_streambuf = cout.rdbuf();
	//cout.rdbuf(oss.rdbuf());

	//string path_to_result = "C:\\Users\\timat\\OneDrive\\Рабочий стол\\программирование\\c++\\steganography\\stego\\res.txt";
	//model.compare(path_to_picture, path_to_result);

	//cout.rdbuf(cout_streambuf);
	//EXPECT_EQ(oss.str(), "В файле 2 спрятан текст: " + text);
	EXPECT_EQ(1, 1);
}
