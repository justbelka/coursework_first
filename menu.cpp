#include <iostream>
#include <windows.h>
#include <string>
#include <conio.h>
#include "menu.h"
#include "cursor.h"
#include "config.h"


BaseMenu::BaseMenu(int _x, int _y, const std::vector<std::string>& InputOptions) {
	this->options = InputOptions;
	this->x = _x;
	this->y = _y;
}

void BaseMenu::PrintOptions(int active) {
	Cursor cursor;
	int cx = this->x;
	int cy = this->y;
	for (int i = 0; i < size(this->options); i++) {
		if (i < 16) {
			cursor.SetPosition(cx, cy);
		}
		else {
			cy = cy % 16;
			if (cy < this->y) cy += 16;
			cursor.SetPosition(cx + 50, cy);
		}
		if (i == active) {
			cursor.SetTextColor(39);
		}
		std::cout << this->options[i] << std::endl;
		cursor.SetTextColor(7);
		cy++;
	}
}

int BaseMenu::run(int exit_function) {
	Cursor cursor;
	cursor.SetVisible(false, 100);

	short active = 0;
	char ch;
	while (true) {
		this->PrintOptions(active);

		ch = _getch();
		if (ch == -32) ch = _getch();
		switch (ch) {
		case KEY_UP:
			if (active > 0)
				active--;
			break;
		case KEY_DOWN:
			if (active < size(this->options) - 1)
				active++;
			break;
		case KEY_RIGHT:
			if (active > 0)
				active--;
			break;
		case KEY_LEFT:
			if (active < size(this->options) - 1)
				active++;
			break;
		case KEY_ESCAPE:
			if (exit_function == 0) return 9999;
			else if (exit_function == 1) return 1000;
			break;
		case KEY_ENTER:
			return active;
			break;
		}
	}
}

void HorizontalMenu::PrintOptions(int active) {
	Cursor cursor;
	int cx = this->x;
	int cy = this->y;
	for (int i = 0; i < size(this->options); i++) {
		cursor.SetPosition(cx, cy);
		if (i == active) {
			cursor.SetTextColor(39);
		}
		std::cout << this->options[i] << std::endl;
		cursor.SetTextColor(7);
		cx += this->options[i].size();
	}
}

void ListingMenu::PrintOptions(int active) {
	Cursor cursor;
	int cx = this->x;
	int cy = this->y;
	if (active < 15) this->global_start = 0;
	else this->global_start = active - 14;

	if (this->options.size() > 15) {
		for (int i = this->global_start; i < this->global_start + 15; i++) {
			cursor.SetPosition(cx, cy);
			if (i == active) {
				cursor.SetTextColor(103);
			}
			if (i == this->options.size()) break;
			std::cout << this->options[i] << std::endl;
			cursor.SetTextColor(7);
			cy++;
		}
	}
	else {
		for (int i = 0; i < this->options.size(); i++) {
			cursor.SetPosition(cx, cy);
			if (i == active) {
				cursor.SetTextColor(103);
			}
			std::cout << this->options[i] << std::endl;
			cursor.SetTextColor(7);
			cy++;
		}
	}
}
