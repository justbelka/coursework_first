#include "cursor.h"
#include <iostream>
#include <conio.h>


void Cursor::SetConsoleName(std::string title) {
    std::string command = std::string("title ") + title;
    system(command.c_str());
}

void Cursor::SetPosition(short x, short y) {
	SetConsoleCursorPosition(Cursor::hstdout, { x, y });
}

void Cursor::SetVisible(bool show, short size) {
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(Cursor::hstdout, &CursorInfo);
	CursorInfo.bVisible = show;
	CursorInfo.dwSize = size;
	SetConsoleCursorInfo(Cursor::hstdout, &CursorInfo);
}

void Cursor::SetConsoleSize(short x, short y) {    
    COORD crd = { x, y };
	SMALL_RECT src = { 0, 0, crd.X, crd.Y };
	SetConsoleWindowInfo(Cursor::hstdout, true, &src);
	SetConsoleScreenBufferSize(Cursor::hstdout, crd);
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
}

void Cursor::SetTextColor(int color) {
    SetConsoleTextAttribute(Cursor::hstdout, color);
}

void Cursor::Alert(std::string text) {
    Cursor::ClearConsole();
    Cursor::SetPosition(35, 14);
    Cursor::SetTextColor(2);
    std::cout << text;
    Cursor::SetTextColor(7);
    _getch();
}

void Cursor::ClearConsole() {
    COORD topLeft = { 0, 0 };
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;
    GetConsoleScreenBufferInfo(Cursor::hstdout, &screen);
    FillConsoleOutputCharacter(Cursor::hstdout, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    FillConsoleOutputAttribute(Cursor::hstdout, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE, screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    SetPosition(0, 0);
}
