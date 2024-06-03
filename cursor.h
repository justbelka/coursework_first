#pragma once
#include <iostream>
#include <windows.h>
#include <string>


class Cursor {
private:
	HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
public:
	void SetConsoleName(std::string title);
	void SetPosition(short x, short y);
	void SetVisible(bool show, short size);
	void SetConsoleSize(short x, short y);
	void SetTextColor(int color);
	void Alert(std::string text);
	void ClearConsole();
};
