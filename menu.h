#pragma once
#include <string>
#include <vector>

class BaseMenu {
protected:
	int x, y;
	std::vector<std::string> options;
public:
	BaseMenu(int _x, int _y, const std::vector<std::string>& InputOptions);
	virtual void PrintOptions(int active);
	int run(int exit_function = 0);
};


class HorizontalMenu : public BaseMenu {
public:
	using BaseMenu::BaseMenu;
	void PrintOptions(int active) override;
	using BaseMenu::run;
};


class ListingMenu : public BaseMenu {
public:
	int global_start = 0;
	using BaseMenu::BaseMenu;
	void PrintOptions(int active) override;
	using BaseMenu::run;
};
