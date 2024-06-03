#include "date.h"
#include <chrono>
#include <format>
#include <string>


Date::Date() {
	this->day = 1;
	this->month = 1;
	this->year = 2001;
}

Date::Date(unsigned short int _day, unsigned short int _month, unsigned short int _year) {
	this->day = _day;
	this->month = _month;
	this->year = _year;
}

void Date::SetDate(unsigned short int _day, unsigned short int _month, unsigned short int _year) {
	this->day = _day;
	this->month = _month;
	this->year = _year;
}

bool Date::CheckDate() {
	int current_year{ std::stoi(std::format("{:%Y}", std::chrono::system_clock::now())) };
	if ((this->day > 0) and (this->day <= 31) and
		(this->month > 0) and (this->month <= 12) and
		(this->year >= 1947) and (this->year <= current_year)) {
		return 1;
	}
	else {
		return 0;
	}
}

std::string Date::GetDate() {
	std::string result;
	if (this->day < 10) {
		result = "0" + std::to_string(this->day) + this->delimiter;
	}
	else {
		result = std::to_string(this->day) + this->delimiter;
	}
	if (this->month < 10) {
		result = result + "0" + std::to_string(this->month) + this->delimiter;
	}
	else {
		result = result + std::to_string(this->month) + this->delimiter;
	}
	return result + std::to_string(this->year);
}
