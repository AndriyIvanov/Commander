#pragma once

#include <vector>
#include <utility>

#include "Button.h"
#include "Window.h"

class Commander
{
	Window MainWindow_;
	vector<Button> MenuButtons_;
	pair<short, short> CursorPosition_;						//Текущее положение курсора (x, y)
	string PathLeft_;										//Адрес левой панели
	string PathRight_;										//Адрес правой панели
	string RootPathLeft_;									//Корневой адрес левой панели
	string RootPathRight_;									//Корневой адрес правой панели
	vector<pair<bool, string>> LeftFolderContent_;			//Содержимое левой панели
	vector<pair<bool, string>> RightFolderContent_;			//Содержимое правой панели
	short MaxNumber_;										//Максимальное кол-во строк в панели окна
	short MinLineLimit_;									//Координата Y первой строки списка окна
	short MaxLineLimitLeft_;								//Координата Y последней строки списка левого окна
	short MaxLineLimitRight_;								//Координата Y последней строки списка правого окна
	void SetPathLeft(string path);
	void SetPathRight(string path);
	short ShiftLeft_;										//Величина сдвига левой панели (при прокрутке в окне)
	short ShiftRight_;										//Величина сдвига правой панели (при прокрутке в окне)
	void FindRoots();
	vector<pair<bool, string>> FolderContent(string path);	//false = файл, true = папка, string = путь к родительской папке (заканчивается \)
	void PrintFolderList(short x, short y, vector<pair<bool, string>> Content, size_t Begin, size_t End);
	void ShowMainWindow();									//Отрисовка главного окна
	int WaitingForKey();									//Ожидание нажатия клавиши
	void ShowCursor(bool colored);							//Показ текущего курсора (colored = true - выделение цветом)
	void ClearPanel(short x, short y);
	bool copyFile(string source, string destination);       //Копирование файла
	void copyDir(string beginDir, string endDir, string newDirName); //Копирование папки, beginDir, endDir - пути к начальной и конечной папкам, соответственно
	string newPath(string oldPath, string newDir);			// Возвращает новый путь с добавленным подкатологом newDir
	void deleteDir(string path);							//Удаление папки,  path - путь к папке для удаления 
	void ActionKeyDownArrow();
	void ActionKeyUpArrow();
	void ActionKeyLeftArrow();
	void ActionKeyRightArrow();
	void ActionKeyTab();
	void ActionKeyEnter();
	void ActionKeyF9();			//Parrent
	void ActionKeyF5();			//Copie
	void ActionKeyF8();			//Delete
	void ActionKeyF4();			//Move
	void ActionKeyF2();			//Change Disk
public:
	Commander();	
	void KeyProcessing();
};

