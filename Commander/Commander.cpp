#include "Commander.h"
#include "ChooseDiskWindow.h"
#include <windows.h>
#include <iostream>
#include <vector>
#include <conio.h>
#include <iomanip>
#include <io.h>
#include <direct.h>
#include "ChooseDiskWindow.h"

using namespace std;

Commander::Commander()
{
	MaxNumber_ = 36;
	MinLineLimit_ = 3;
	ShiftLeft_ = 0;
	ShiftRight_ = 0;
	FindRoots();
	short x = 1, y = 41, dx = 10;
	MenuButtons_.push_back(Button(x, y, "F1       "));
	x += dx;
	MenuButtons_.push_back(Button(x, y, "F2 CD    "));
	x += dx;
	MenuButtons_.push_back(Button(x, y, "F3       "));
	x += dx;
	MenuButtons_.push_back(Button(x, y, "F4 Move  "));
	x += dx;
	MenuButtons_.push_back(Button(x, y, "F5 Copy  "));
	x += dx;
	MenuButtons_.push_back(Button(x, y, "F6       "));
	x += dx;
	MenuButtons_.push_back(Button(x, y, "F7       "));
	x += dx;
	MenuButtons_.push_back(Button(x, y, "F8 Delete"));
	x += dx;
	MenuButtons_.push_back(Button(x, y, "F9 Parent"));
	x += dx;
	MenuButtons_.push_back(Button(x, y, "F10 Quit "));
	x += dx;
	MenuButtons_.push_back(Button(x, y, "F11      "));
	x += dx;
	MenuButtons_.push_back(Button(x, y, "F12      "));
	CursorPosition_ = { 1, 3 };				 //Начальное положение курсора
	PathLeft_ = RootPathLeft_;
	PathRight_ = RootPathRight_;
	LeftFolderContent_ = FolderContent(PathLeft_);
	RightFolderContent_ = FolderContent(PathRight_);
	MaxLineLimitLeft_ = (short) LeftFolderContent_.size() + 2;
	MaxLineLimitRight_ = (short) RightFolderContent_.size() + 2;
	ShowMainWindow();
}

void Commander::FindRoots()
{
	int n;
	vector<char> letters;
	DWORD dr = GetLogicalDrives();			//получаем битовую маску доступных дисков
	for (int i = 0; i < 26; i++)			//потому что всего 26 букв
	{
		n = ((dr >> i) & 0x00000001);		//сдвигаемся по битам
		if (n == 1) 
			letters.push_back(char(65 + i)); 
	}
	RootPathLeft_.clear();
	RootPathLeft_.push_back(letters[0]);
	RootPathLeft_+=":\\";
	RootPathRight_.clear();
	if (letters.size() > 1) RootPathRight_.push_back(letters[1]);
	else RootPathRight_.push_back(letters[0]);
	RootPathRight_ += ":\\";
}

vector<pair<bool, string>> Commander::FolderContent(string path) 
{
	path += "*";
	WIN32_FIND_DATA FindFileData;
	HANDLE hf;
	pair<bool, string> FolderItem;
	vector<pair<bool, string>> Files, Folders;
	hf = FindFirstFile(path.c_str(), &FindFileData);
	if (hf != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (strcmp(FindFileData.cFileName, ".") == 0 || strcmp(FindFileData.cFileName, "..") == 0) continue;
			FolderItem.second = FindFileData.cFileName;
			if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)==0)
			{
				FolderItem.first = false;						//файл
				Files.push_back(FolderItem);
			}
			else
			{			
				FolderItem.first = true;						//папка
				Folders.push_back(FolderItem);
			}
		} while (FindNextFile(hf, &FindFileData) != 0);
		FindClose(hf);
	}
	Folders.insert(Folders.end(), Files.begin(), Files.end());
	if (Folders.empty())
	{
		FolderItem.first = false;
		FolderItem.second = "...";
		Folders.push_back(FolderItem);
	}
	return Folders;
}

void Commander::ShowMainWindow()
{
	MainWindow_.PrintWindow();							//Печать окна
	for (auto it : MenuButtons_)
		it.PrintButton();								//Печать кнопок меню
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursor = { 1, false };	
	SetConsoleCursorInfo(console, &cursor);				//Делаем невидимым курсор
	COORD coord{1, 1};
	SetConsoleCursorPosition(console, coord);
	if (PathLeft_.size() < 58)
		cout << PathLeft_;
	else
		for (size_t j = 0; j < 58; ++j)
			cout << PathLeft_[j];
	coord.X = 60;
	SetConsoleCursorPosition(console, coord);
	if (PathRight_.size() < 58)
		cout << PathRight_;
	else
		for (size_t j = 0; j < 58; ++j)
			cout << PathRight_[j];
	PrintFolderList(1, 3, LeftFolderContent_, 0, MaxNumber_);
	PrintFolderList(60, 3, RightFolderContent_, 0, MaxNumber_);
	short x = 1, y = 41;
	ShowCursor(true);
}

void Commander::SetPathLeft(string path)
{
	PathLeft_= PathLeft_ + path + "\\";
}

void Commander::SetPathRight(string path)
{
	PathRight_ = PathRight_ + path + "\\";
}

void Commander::PrintFolderList(short x, short y, vector<pair<bool, string>> Content, size_t Begin, size_t End)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord = { x, y };
	if (Content.empty()) return;
	for (size_t i = Begin; i < Content.size() && i < End; ++i)
	{
		SetConsoleCursorPosition(console, coord);
		if (Content[i].first) cout << "DIR> ";
		if (Content[i].second.size() < 53)
			cout << Content[i].second << endl;
		else
			for (size_t j = 0; j < 53; ++j)
				cout << Content[i].second[j];
		cout << endl;
		coord.Y += 1;
	}
}

void Commander::ClearPanel(short x, short y)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord = { x, y };
	for (short i = 0; i < MaxNumber_; ++i)
	{
		SetConsoleCursorPosition(console, coord);
		cout << " " << setw(57) << setfill(' ') << " " << endl;
		coord.Y += 1;
	}
}

bool Commander::copyFile(string source, string destination)
{
	const int size = 65536;	
	FILE *Fread; 													//Открытие Файла чля чтения
	errno_t err = fopen_s(&Fread, source.c_str(), "rb");
	if (err != 0)
	{
		cerr << "Ошибка открытия файла : " << err << "\n";
		return false;
	}
	int fileSize = (int)_filelength(_fileno(Fread));
	int bufSize = fileSize / sizeof(char);

	FILE *Fwrite;													//Открытие Файла чля записи
	err = fopen_s(&Fwrite, destination.c_str(), "wb");
	if (err != 0)
	{
		cerr << "Ошибка открытия файла : " << err << "\n";
		return false;
	}
	char *data = new char[fileSize];								//Буфер
	while (!feof(Fread))
	{
		size_t realsize = fread_s(data, fileSize, sizeof(char), bufSize, Fread);
		fwrite(data, sizeof(char), realsize, Fwrite);
	}
	delete[] data;
	fclose(Fread);
	fclose(Fwrite);
	return true;
}

string Commander::newPath(string oldPath, string newDir)
{
	string newPath;
	for (auto i : oldPath)
		if (i != '*') newPath.push_back(i);
	newPath += newDir;
	return newPath;
}

void Commander::copyDir(string beginDir, string endDir, string newDirName)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hf;
	endDir += '\\' + newDirName;
	if (_mkdir(endDir.c_str()) == -1)
	{
		cerr << "Ошибка!\n Невозвожно создать папку.\n";
		return;
	}
	beginDir += "\\*";
	hf = FindFirstFile(beginDir.c_str(), &FindFileData);
	if (hf != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (strcmp(FindFileData.cFileName, ".") == 0 || strcmp(FindFileData.cFileName, "..") == 0)
				continue;
			string fileName{ FindFileData.cFileName };
			if (FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
			{
				string source = newPath(beginDir, fileName);
				copyDir(source, endDir, fileName);
			}
			else
			{
				string source = newPath(beginDir, fileName);
				string destination{ endDir + "\\" + fileName };
				copyFile(source, destination);
			}

		} while (FindNextFile(hf, &FindFileData) != 0);
		FindClose(hf);
	}
}

void Commander::deleteDir(string path)				//path - путь к папке для удаления 
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hf;

	string beginDir = path + "\\*";
	hf = FindFirstFile(beginDir.c_str(), &FindFileData);
	if (hf != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (strcmp(FindFileData.cFileName, ".") == 0 || strcmp(FindFileData.cFileName, "..") == 0)
				continue;
			string fileName{ FindFileData.cFileName };
			if (FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
			{
				string source = newPath(beginDir, fileName);
				deleteDir(source);
			}
			else
			{
				string source = newPath(beginDir, fileName);
				if (remove(source.c_str()) != 0)
					cerr << "Ошибка!\n Невозвожно удалить файл " << source << "\n";
			}

		} while (FindNextFile(hf, &FindFileData) != 0);
		FindClose(hf);
	}
	if (_rmdir(path.c_str()) == -1)
		cerr << "Ошибка!\n Невозвожно удалить папку " << path << "\n";
}


void Commander::ShowCursor(bool colored)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord = { CursorPosition_.first, CursorPosition_.second };
	SetConsoleCursorPosition(console, coord);
	if (colored) SetConsoleTextAttribute(console, (WORD)((14 << 4) | 0));  //Цвет выделения строки (черный шрифт на желтом фоне)
	pair<bool, string> Temp;
	if (CursorPosition_.first < 59)
		Temp = LeftFolderContent_[CursorPosition_.second - 3+ ShiftLeft_];
	else
		Temp = RightFolderContent_[CursorPosition_.second - 3+ ShiftRight_];
	if (Temp.first) cout << "DIR> ";
	if (Temp.second.size() < 53)
		cout << Temp.second;
	else
		for (size_t j = 0; j < 53; ++j)
			cout << Temp.second[j];

	if (colored) SetConsoleTextAttribute(console, (WORD)((9 << 4) | 15));  //Возврат к основным цветам (белый шрифт на синем фоне)
}

int Commander::WaitingForKey()
{
	while (true)
	{
		int button = _getch();
		if (button == 72 || button == 75 || button == 77 || button == 80 ||  // Стрелки курсора
			button == 9 ||													 // Tab
			button == 13 ||													 // Enter
			(button > 58 && button < 71))									 // Функциональные клавиши (F1 - F12)
			return button;
	}
}

void Commander::KeyProcessing()
{
	while(true)
	{
		int button = WaitingForKey();
		switch (button)
		{
			case 9:  //Tab
			{
				ActionKeyTab();
				break;
			}
			case 13:  //Enter
			{
				ActionKeyEnter();
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
			case 72: //Стрелка вверх
			{
				ActionKeyUpArrow();
				break;
			}
			case 80: //Стрелка вниз
			{
				ActionKeyDownArrow();
				break;
			}
			case 60: //F2
			{
				ActionKeyF2();
				break;
			}
			case 62: //F4
			{
				ActionKeyF4();
				break;
			}
			case 63: //F5
			{
				ActionKeyF5();
				break;
			}
			case 66: //F8
			{
				ActionKeyF8();
				break;
			}
			case 67: //F9
			{
				ActionKeyF9();
				break;
			}
			case 68: //F10
			{
				return;
			}
		}
	}
}

void Commander::ActionKeyDownArrow()
{
	short LastLinePosition;
	if (CursorPosition_.first < 59) LastLinePosition = MaxLineLimitLeft_;
	else LastLinePosition = MaxLineLimitRight_;
	if (LastLinePosition > (MaxNumber_+2)) LastLinePosition = MaxNumber_+2;

	if (CursorPosition_.second != LastLinePosition)
	{
		ShowCursor(false);
		CursorPosition_.second++;
		ShowCursor(true);
	}
	
	if ((CursorPosition_.second == LastLinePosition) && (CursorPosition_.first < 59) && ((short) LeftFolderContent_.size() > MaxNumber_) && ((short)LeftFolderContent_.size() - MaxNumber_) > ShiftLeft_)
	{
		ShiftLeft_++;
		ClearPanel(1, 3);
		PrintFolderList(1, 3, LeftFolderContent_, ShiftLeft_, MaxNumber_ + ShiftLeft_);
		CursorPosition_.second = MaxNumber_ + 2;
		ShowCursor(true);
	}

	if ((CursorPosition_.second == LastLinePosition) && (CursorPosition_.first > 59) && ((short)RightFolderContent_.size() > MaxNumber_) && ((short)RightFolderContent_.size() - MaxNumber_) > ShiftRight_)
	{
		ShiftRight_++;
		ClearPanel(60, 3);
		PrintFolderList(60, 3, RightFolderContent_, ShiftRight_, MaxNumber_ + ShiftRight_);
		CursorPosition_.second = MaxNumber_ + 2;
		ShowCursor(true);
	}
}

void Commander::ActionKeyUpArrow()
{
	if (CursorPosition_.second != MinLineLimit_)
	{
		ShowCursor(false);
		CursorPosition_.second--;
		ShowCursor(true);
	}
	if ((CursorPosition_.second == MinLineLimit_) && (CursorPosition_.first < 59) && ((short)LeftFolderContent_.size() > MaxNumber_) && (ShiftLeft_ > 0))
	{
		ShiftLeft_--;
		ClearPanel(1, 3);
		PrintFolderList(1, 3, LeftFolderContent_, ShiftLeft_, MaxNumber_ + ShiftLeft_);
		CursorPosition_.second = MinLineLimit_;
		ShowCursor(true);
	}
	if ((CursorPosition_.second == MinLineLimit_) && (CursorPosition_.first > 59) && ((short)RightFolderContent_.size() > MaxNumber_) && (ShiftRight_ > 0))
	{
		ShiftRight_--;
		ClearPanel(60, 3);
		PrintFolderList(60, 3, RightFolderContent_, ShiftRight_, MaxNumber_ + ShiftRight_);
		CursorPosition_.second = MinLineLimit_;
		ShowCursor(true);
	}
}

void Commander::ActionKeyRightArrow()
{
	if (CursorPosition_.first < 59)
	{
		ShowCursor(false);
		CursorPosition_ = { 60, 3 };
		ShowCursor(true);
	}
}

void Commander::ActionKeyLeftArrow()
{
	if (CursorPosition_.first > 59)
	{		
		ShowCursor(false);
		CursorPosition_ = { 1, 3 };
		ShowCursor(true);
	}
}

void Commander::ActionKeyTab()
{
	if (CursorPosition_.first > 59)
	{
		ShowCursor(false);
		CursorPosition_ = { 1, 3 };
		ShowCursor(true);
		return;
	}
	ShowCursor(false);
	CursorPosition_ = { 60, 3 };
	ShowCursor(true);
}

void Commander::ActionKeyEnter()
{
	pair<bool, string> Temp;
	if (CursorPosition_.first < 59)
	{
		Temp = LeftFolderContent_[CursorPosition_.second - 3 + ShiftLeft_];
		if (!Temp.first) return;
		SetPathLeft(Temp.second);
		LeftFolderContent_ = FolderContent(PathLeft_);
		MaxLineLimitLeft_ = (short)LeftFolderContent_.size() + 2;
		ShiftLeft_ = 0;
		CursorPosition_ = { 1, 3 };
	}
	else
	{
		Temp = RightFolderContent_[CursorPosition_.second - 3 + ShiftRight_];
		if (!Temp.first) return;
		SetPathRight(Temp.second);
		RightFolderContent_ = FolderContent(PathRight_);
		MaxLineLimitRight_ = (short)RightFolderContent_.size() + 2;
		ShiftRight_ = 0;
		CursorPosition_ = { 60, 3 };
	}
	system("cls");
	ShowMainWindow();
}

void Commander::ActionKeyF9()						 //Parent
{
	string Temp;
	if (CursorPosition_.first < 59)						
		if (PathLeft_== RootPathLeft_) return;
		else Temp = PathLeft_;
	else
		if (PathRight_ == RootPathRight_) return;
		else Temp = PathRight_;
	
	do
	{
		Temp.pop_back();
	} while (Temp.back() != '\\');

	if (CursorPosition_.first < 59)
	{
		PathLeft_ = Temp;
		LeftFolderContent_ = FolderContent(PathLeft_);
		MaxLineLimitLeft_ = (short)LeftFolderContent_.size() + 2;
		ShiftLeft_ = 0;
		CursorPosition_ = { 1, 3 };
	}
	else
	{
		PathRight_ = Temp;
		RightFolderContent_ = FolderContent(PathRight_);
		MaxLineLimitRight_ = (short)RightFolderContent_.size() + 2;
		ShiftRight_ = 0;
		CursorPosition_ = { 60, 3 };
	}
	system("cls");
	ShowMainWindow();
}

void Commander::ActionKeyF5()						//Copie
{
	pair<bool, string> Temp;
	string endDir, newDirName;
	if (CursorPosition_.first < 59)
	{
		Temp = LeftFolderContent_[CursorPosition_.second - 3 + ShiftLeft_];
		endDir = PathRight_;
		if (!Temp.first)
		{
			copyFile(PathLeft_ + Temp.second, endDir + Temp.second);
		}
		else
		{
			endDir.pop_back();
			newDirName = Temp.second;
			copyDir(PathLeft_ + Temp.second, endDir, newDirName);
		}
		RightFolderContent_ = FolderContent(PathRight_);
		MaxLineLimitRight_ = (short)RightFolderContent_.size() + 2;
		ShiftRight_ = 0;
		CursorPosition_ = { 1, 3 };
	}
	else
	{
		Temp = RightFolderContent_[CursorPosition_.second - 3 + ShiftRight_];
		endDir = PathLeft_;
		if (!Temp.first)
		{
			copyFile(PathRight_ + Temp.second, endDir + Temp.second);
		}
		else
		{
			endDir.pop_back();
			newDirName = Temp.second;
			copyDir(PathRight_ + Temp.second, endDir, newDirName);
		}
		LeftFolderContent_ = FolderContent(PathLeft_);
		MaxLineLimitLeft_ = (short)LeftFolderContent_.size() + 2;
		ShiftLeft_ = 0;
		CursorPosition_ = { 60, 3 };
	}
	system("cls");
	ShowMainWindow();
}

void Commander::ActionKeyF8()						//Delete
{
	pair<bool, string> Temp;
	if (CursorPosition_.first < 59)
	{
		Temp = LeftFolderContent_[CursorPosition_.second - 3 + ShiftLeft_];
		if (!Temp.first)
		{
			remove((PathLeft_ + Temp.second).c_str());
		}
		else
		{
			deleteDir(PathLeft_ + Temp.second);
		}
		LeftFolderContent_ = FolderContent(PathLeft_);
		MaxLineLimitLeft_ = (short)LeftFolderContent_.size() + 2;
		ShiftLeft_ = 0;
		CursorPosition_ = { 1, 3 };
	}
	else
	{
		Temp = RightFolderContent_[CursorPosition_.second - 3 + ShiftRight_];
		if (!Temp.first)
		{
			remove((PathRight_ + Temp.second).c_str());
		}
		else
		{
			deleteDir(PathRight_ + Temp.second);
		}
		RightFolderContent_ = FolderContent(PathRight_);
		MaxLineLimitRight_ = (short)RightFolderContent_.size() + 2;
		ShiftRight_ = 0;
		CursorPosition_ = { 60, 3 };
	}
	system("cls");
	ShowMainWindow();
}


void Commander::ActionKeyF4()						//Move
{
	pair<bool, string> Temp;
	string endDir, newDirName;
	if (CursorPosition_.first < 59)
	{
		Temp = LeftFolderContent_[CursorPosition_.second - 3 + ShiftLeft_];
		endDir = PathRight_;
		if (!Temp.first)
		{
			copyFile(PathLeft_ + Temp.second, endDir + Temp.second);
			remove((PathLeft_ + Temp.second).c_str());
		}
		else
		{
			endDir.pop_back();
			newDirName = Temp.second;
			copyDir(PathLeft_ + Temp.second, endDir, newDirName);
			deleteDir(PathLeft_ + Temp.second);
		}
		CursorPosition_ = { 1, 3 };
	}
	else
	{
		Temp = RightFolderContent_[CursorPosition_.second - 3 + ShiftRight_];
		endDir = PathLeft_;
		if (!Temp.first)
		{
			copyFile(PathRight_ + Temp.second, endDir + Temp.second);
			remove((PathRight_ + Temp.second).c_str());
		}
		else
		{
			endDir.pop_back();
			newDirName = Temp.second;
			copyDir(PathRight_ + Temp.second, endDir, newDirName);
			deleteDir(PathRight_ + Temp.second);
		}
		CursorPosition_ = { 60, 3 };
	}
	RightFolderContent_ = FolderContent(PathRight_);
	LeftFolderContent_ = FolderContent(PathLeft_);
	MaxLineLimitRight_ = (short)RightFolderContent_.size() + 2;
	MaxLineLimitLeft_ = (short)LeftFolderContent_.size() + 2;
	ShiftRight_ = 0;
	ShiftLeft_ = 0;
	system("cls");
	ShowMainWindow();
}

void Commander::ActionKeyF2()						//Change Disk
{
	if (CursorPosition_.first < 59)
	{
		ChooseDiskWindow ChooseWindow(15, 8, RootPathLeft_[0]);
		ChooseWindow.PrintWindow();

		RootPathLeft_.clear();
		RootPathLeft_.push_back(ChooseWindow.GetChosedDisk());
		RootPathLeft_ += ":\\";
		PathLeft_ = RootPathLeft_;

		LeftFolderContent_ = FolderContent(PathLeft_);
		MaxLineLimitLeft_ = (short)LeftFolderContent_.size() + 2;
		ShiftLeft_ = 0;
		CursorPosition_ = { 1, 3 };
	}
	else
	{
		ChooseDiskWindow ChooseWindow(74, 8, RootPathRight_[0]);
		ChooseWindow.PrintWindow();

		RootPathRight_.clear();
		RootPathRight_.push_back(ChooseWindow.GetChosedDisk());
		RootPathRight_ += ":\\";
		PathRight_ = RootPathRight_;

		RightFolderContent_ = FolderContent(PathRight_);
		MaxLineLimitRight_ = (short)RightFolderContent_.size() + 2;
		ShiftRight_ = 0;
		CursorPosition_ = { 60, 3 };
	}
	system("cls");
	ShowMainWindow();
}
