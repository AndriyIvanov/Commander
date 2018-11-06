#pragma once
#include <string>

using namespace std;

class Button
{
	short x_;		//Координата кнопки по горизонтали
	short y_;		//Координата кнопки по вертикали
	string name_;	//Надпись кнопки
public:
	Button(short x, short y, string name);
	void PrintButton();
};

