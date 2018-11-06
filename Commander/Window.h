#pragma once

class Window
{
	size_t Height_;
	size_t Width_;
public:
	Window(size_t Height= 44, size_t Width=121) :  Height_(Height), Width_(Width)	{};
	void PrintWindow();
};

