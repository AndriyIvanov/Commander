#pragma once

#include <windows.h>
#include <vector>

using namespace std;

class ChooseDiskWindow
{
	size_t Height_;
	size_t Width_;
	short X_;								//���������� � ������ �������� ����
	short Y_;								//���������� Y ������ �������� ����
	short Background_;						//���� ���� ����
	short FontColour_;						//���� ������
	vector<char> letters_;					//��������� �����
	char ChoosedDisk_;						//��������� ����
	pair<short, short> CursorPosition_;		//������� ��������� ������� (x, y)

	void ChooseDisk_();						//����� �����
	void ActionKeyLeftArrow();
	void ActionKeyRightArrow();
	void ActionKeyEnter();
	void ShowCursor(bool colored);
	int WaitingForKey();
	void KeyProcessing();

public:
//	ChooseDiskWindow(short X, short Y, size_t Height, size_t Width, short color) : Height_(Height), Width_(Width), X_(X), Y_(Y), color_(color) { };
	ChooseDiskWindow(short X, short Y, char CurrentDisk);
	COORD getCoord() const;
	short GetBackground() const;
	short GetFontColour() const;
	char GetChosedDisk() const;
	void PrintWindow();

};

