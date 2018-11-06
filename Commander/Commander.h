#pragma once

#include <vector>
#include <utility>

#include "Button.h"
#include "Window.h"

class Commander
{
	Window MainWindow_;
	vector<Button> MenuButtons_;
	pair<short, short> CursorPosition_;						//������� ��������� ������� (x, y)
	string PathLeft_;										//����� ����� ������
	string PathRight_;										//����� ������ ������
	string RootPathLeft_;									//�������� ����� ����� ������
	string RootPathRight_;									//�������� ����� ������ ������
	vector<pair<bool, string>> LeftFolderContent_;			//���������� ����� ������
	vector<pair<bool, string>> RightFolderContent_;			//���������� ������ ������
	short MaxNumber_;										//������������ ���-�� ����� � ������ ����
	short MinLineLimit_;									//���������� Y ������ ������ ������ ����
	short MaxLineLimitLeft_;								//���������� Y ��������� ������ ������ ������ ����
	short MaxLineLimitRight_;								//���������� Y ��������� ������ ������ ������� ����
	void SetPathLeft(string path);
	void SetPathRight(string path);
	short ShiftLeft_;										//�������� ������ ����� ������ (��� ��������� � ����)
	short ShiftRight_;										//�������� ������ ������ ������ (��� ��������� � ����)
	void FindRoots();
	vector<pair<bool, string>> FolderContent(string path);	//false = ����, true = �����, string = ���� � ������������ ����� (������������� \)
	void PrintFolderList(short x, short y, vector<pair<bool, string>> Content, size_t Begin, size_t End);
	void ShowMainWindow();									//��������� �������� ����
	int WaitingForKey();									//�������� ������� �������
	void ShowCursor(bool colored);							//����� �������� ������� (colored = true - ��������� ������)
	void ClearPanel(short x, short y);
	bool copyFile(string source, string destination);       //����������� �����
	void copyDir(string beginDir, string endDir, string newDirName); //����������� �����, beginDir, endDir - ���� � ��������� � �������� ������, ��������������
	string newPath(string oldPath, string newDir);			// ���������� ����� ���� � ����������� ������������ newDir
	void deleteDir(string path);							//�������� �����,  path - ���� � ����� ��� �������� 
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

