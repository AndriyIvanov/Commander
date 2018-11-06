#include "ChooseDiskWindow.h"

#include <string>
#include <iomanip>
#include <conio.h>
#include <iostream>

ChooseDiskWindow::ChooseDiskWindow(short X, short Y, char CurrentDisk)
{
	Height_ = 6;		//Высота окна
	Width_ = 27;		//Ширина окна
	X_ = X;				//Координата Х левого верхнего угла
	Y_ = Y;				//Координата Y левого верхнего угла
	Background_ = 4;	//Цвет фона - бордовый
	FontColour_ = 14;	//Цвет шрифта - желтый
	ChooseDisk_();
	ChoosedDisk_ = CurrentDisk;
}

COORD ChooseDiskWindow::getCoord() const
{	
	return COORD{ X_, Y_ };
}

short ChooseDiskWindow::GetBackground() const
{
	return Background_;
}

short ChooseDiskWindow::GetFontColour() const
{
	return FontColour_;
}

char ChooseDiskWindow::GetChosedDisk() const
{
	return ChoosedDisk_;
}

void ChooseDiskWindow::PrintWindow()
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console, (WORD)((Background_ << 4) | FontColour_));  //Установка основного цвета окна

	COORD coord = { X_, Y_ };	

	SetConsoleCursorPosition(console, coord);
	for (size_t i = 0; i < Height_; ++i)
	{
		SetConsoleCursorPosition(console, coord);
		cout << " " << setw(Width_-2) << setfill(' ') << " " << endl;
		coord.Y++;
	}
	coord.Y = Y_+1;
	coord.X = X_+1;
	SetConsoleCursorPosition(console, coord);
	cout << "Выберите диск:";

	coord.X = X_ + 1;
	coord.Y += 2;
	SetConsoleCursorPosition(console, coord);
	for (auto it : letters_)
		cout << it << ":  ";
	CursorPosition_.first = X_ + 1;
	CursorPosition_.second = coord.Y;

	ShowCursor(true);
	KeyProcessing();

	SetConsoleTextAttribute(console, (WORD)((9 << 4) | 15));  //Возврат к цветам главного окна (белый шрифт на синем фоне)
}

void ChooseDiskWindow::ChooseDisk_()
{
	int n;
	letters_.clear();
	DWORD dr = GetLogicalDrives();			//получаем битовую маску доступных дисков
	for (int i = 0; i < 26; i++)			//потому что всего 26 букв
	{
		n = ((dr >> i) & 0x00000001);		//сдвигаемся по битам
		if (n == 1)
			letters_.push_back(char(65 + i));
	}
//	ChoosedDisk_ = letters_[0];
}


void ChooseDiskWindow::ShowCursor(bool colored)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord = { CursorPosition_.first, CursorPosition_.second };
	SetConsoleCursorPosition(console, coord);
	if (colored) SetConsoleTextAttribute(console, (WORD)((14 << 4) | 0));							//Цвет выделения строки (черный шрифт на желтом фоне)
	cout << letters_[(CursorPosition_.first - X_ - 1)/4] << ":";
	if (colored) SetConsoleTextAttribute(console, (WORD)((Background_ << 4) | FontColour_));		//Возврат к основным цветам окна
}

int ChooseDiskWindow::WaitingForKey()
{
	while (true)
	{
		int button = _getch();
		if (button == 75 || button == 77 ||				 // Стрелки влево и вправо
			button == 13 ||								 // Enter
			button == 27)								 // Esc		 
			return button;
	}
}

void ChooseDiskWindow::KeyProcessing()
{
	bool flag = true;
	while (flag)
	{
		int button = WaitingForKey();
		switch (button)
		{
			case 13:  //Enter
			{
				ActionKeyEnter();
				flag = false;
				break;
			}
			case 27:  //Esc
			{
				flag = false;
				break;
			}

			case 75:  //Стрелка влево
			{
				ActionKeyLeftArrow();
				break;
			}
			case 77:  //Стрелка вправо
			{
				ActionKeyRightArrow();
				break;
			}
		}
	}
}


void ChooseDiskWindow::ActionKeyLeftArrow()
{
	ShowCursor(false);
	if (CursorPosition_.first == (X_ + 1)) CursorPosition_.first = (X_ + 1 + (short) (letters_.size()-1) * 4);
	else CursorPosition_.first -= 4;
	ShowCursor(true);
}

void ChooseDiskWindow::ActionKeyRightArrow()
{
	ShowCursor(false);
	if (CursorPosition_.first == (X_ + 1 + (letters_.size()-1) * 4)) CursorPosition_.first = (X_ + 1);
	else CursorPosition_.first += 4;
	ShowCursor(true);
}

void ChooseDiskWindow::ActionKeyEnter()
{
	ChoosedDisk_ = letters_[(CursorPosition_.first - X_ - 1) / 4];
}
