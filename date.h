#pragma once
#include <string>


class Date {
private:
	unsigned short int day;
	unsigned short int month;
public:
	unsigned short int year;
	char delimiter = '.';
	Date();
	Date(unsigned short int _day, unsigned short int _month, unsigned short int _year);
	void SetDate(unsigned short int _day, unsigned short int _month, unsigned short int _year);
	bool CheckDate();
	std::string GetDate();
};
