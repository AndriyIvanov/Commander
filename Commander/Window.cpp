#include "Window.h"

#include <string>
#include <windows.h>
#include <iostream>
#include <iomanip>

using namespace std;

void Window::PrintWindow()
{
	system("color 9F");								//”становка основных цветов окна (белый шрифт на синем фоне)
	string WindowSize = "mode con cols=";
	WindowSize += to_string(Width_);
	WindowSize += " lines=";
	WindowSize += to_string(Height_);
	system(WindowSize.c_str());						//”становка размера окна

	setlocale(LC_ALL, "C");
	short length = 59;								//Ўирина панели (левой и правой)

	cout << char(201) << setw(length) << setfill(char(205)) << char(203) << setw(length) << setfill(char(205)) << char(187) << endl;
	cout << char(186) << setw(length) << setfill(' ') << char(186) << setw(length) << setfill(' ') << char(186) << endl;
	cout << char(204) << setw(length) << setfill(char(205)) << char(206) << setw(length) << setfill(char(205)) << char(185) << endl;
	for (size_t i = 0; i<36; ++i)
		cout << char(186) << setw(length) << setfill(' ') << char(186) << setw(length) << setfill(' ') << char(186) << endl;
	cout << char(200) << setw(length) << setfill(char(205)) << char(202) << setw(length) << setfill(char(205)) << char(188) << endl;

	setlocale(0, "");
}



