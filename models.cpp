#include "models.h"
#include "cursor.h"
#include "menu.h"
#include <regex>


// Функции группы
bool Group::EnterName() {
	char _name[31];
	Cursor cursor;
	cursor.SetTextColor(7);
	cursor.SetPosition(25, 8);
	cursor.SetVisible(true, 100);
	std::cout << "Enter the group name (XXXX-00-00): ";
	std::cin >> _name;
	std::regex pattern("[А-Я]{4}-\\d{2}-\\d{2}");

	while (!std::regex_match(_name, pattern)) {
		if (std::string(_name) == "exit") {
			return 0;
		}
		cursor.ClearConsole();
		cursor.SetPosition(25, 6);
		cursor.SetTextColor(12);
		std::cout << "Invalid format. Try again";

		cursor.SetPosition(25, 7);
		cursor.SetTextColor(2);
		std::cout << "Creation of new students group (Press \"exit\" to go to the main menu)" << std::endl;
		cursor.SetTextColor(7);
		cursor.SetPosition(25, 8);
		cursor.SetVisible(true, 100);
		std::cout << "Enter the group name (XXXX-00-00): ";
		std::cin >> _name;
	}
	cursor.SetVisible(false, 100);
	strcpy_s(this->name, 11, _name);
	return 1;
}


// Функции студента
bool Student::EnterSurname() {
	int default_position = 8;
	std::string info_alert = "Enter the student's second name: ";
	std::regex pattern("^[А-ЯЁ][а-яё]*(?:[-\s][А-ЯЁ][а-яё]*)*$");
	std::string _surname;
	Cursor cursor;
	cursor.SetTextColor(7);
	cursor.SetPosition(25, default_position);
	cursor.SetVisible(true, 100);
	std::cout << info_alert;
	std::cin >> _surname;

	while (!std::regex_match(_surname, pattern)) {
		if (_surname == "exit") {
			return 0;
		}
		cursor.SetPosition(25, default_position - 2);
		cursor.SetTextColor(12);
		std::cout << "Invalid format. Try again";
		cursor.SetTextColor(7);
		cursor.SetPosition(25 + info_alert.size(), default_position);
		std::cout << std::string(_surname.size(), ' ');
		cursor.SetPosition(25 + info_alert.size(), default_position);
		std::cin.ignore(_surname.size() + 1, '\n');
		std::cin >> _surname;
	}
	cursor.SetVisible(false, 100);
	strcpy_s(this->surname, _surname.c_str());
	return 1;
}

bool Student::EnterName() {
	int default_position = 11;
	std::string info_alert = "Enter the student's first name: ";
	std::regex pattern("^[А-ЯЁ][а-яё]*$");
	std::string _name;
	Cursor cursor;
	cursor.SetTextColor(7);
	cursor.SetPosition(25, default_position);
	cursor.SetVisible(true, 100);
	std::cout << info_alert;
	std::cin >> _name;

	while (!std::regex_match(_name, pattern)) {
		if (_name == "exit") {
			return 0;
		}
		cursor.SetPosition(25, default_position - 2);
		cursor.SetTextColor(12);
		std::cout << "Invalid format. Try again";
		cursor.SetTextColor(7);
		cursor.SetPosition(25 + info_alert.size(), default_position);
		std::cout << std::string(_name.size(), ' ');
		cursor.SetPosition(25 + info_alert.size(), default_position);
		std::cin.ignore(_name.size() + 1, '\n');
		std::cin >> _name;
	}
	cursor.SetVisible(false, 100);
	strcpy_s(this->name, _name.c_str());
	return 1;
}

bool Student::EnterPatronymic() {
	int default_position = 14;
	std::string info_alert = "Enter the student's patronymic: ";
	std::regex pattern("^[А-ЯЁ][а-яё]*$");
	std::string _patronymic;
	Cursor cursor;
	cursor.SetTextColor(7);
	cursor.SetPosition(25, default_position);
	cursor.SetVisible(true, 100);
	std::cout << info_alert;
	std::cin >> _patronymic;

	while (!std::regex_match(_patronymic, pattern)) {
		if (_patronymic == "exit") {
			return 0;
		}
		cursor.SetPosition(25, default_position - 2);
		cursor.SetTextColor(12);
		std::cout << "Invalid format. Try again";
		cursor.SetTextColor(7);
		cursor.SetPosition(25 + info_alert.size(), default_position);
		std::cout << std::string(_patronymic.size(), ' ');
		cursor.SetPosition(25 + info_alert.size(), default_position);
		std::cin.ignore(_patronymic.size() + 1, '\n');
		std::cin >> _patronymic;
	}
	cursor.SetVisible(false, 100);
	strcpy_s(this->patronymic, _patronymic.c_str());
	return 1;
}

bool Student::EnterBirthday() {
	int default_position = 17;
	std::string info_alert = "Enter the student's date of birth (DD-MM-YYYY): ";
	std::regex pattern(R"((0?[1-9]|[12][0-9]|3[01])[-/\\|.](0?[1-9]|1[0-2])[-/\\|.](\d{4}))");
	std::smatch matches;
	std::string _birthday;
	Cursor cursor;
	cursor.SetTextColor(7);
	cursor.SetPosition(25, default_position);
	cursor.SetVisible(true, 100);
	std::cout << info_alert;
	std::cin >> _birthday;

	while (!std::regex_match(_birthday, pattern)) {
		if (_birthday == "exit") {
			return 0;
		}
		cursor.SetPosition(25, default_position - 2);
		cursor.SetTextColor(12);
		std::cout << "Invalid format. Try again  ";
		cursor.SetTextColor(7);
		cursor.SetPosition(25 + info_alert.size(), default_position);
		std::cout << std::string(_birthday.size(), ' ');
		cursor.SetPosition(25 + info_alert.size(), default_position);
		std::cin.ignore(_birthday.size() + 1, '\n');
		std::cin >> _birthday;
	}
	cursor.SetVisible(false, 100);
	std::regex_match(_birthday, matches, pattern);
	int day = std::stoi(matches[1].str());
	int month = std::stoi(matches[2].str());
	int year = std::stoi(matches[3].str());
	this->birthday.SetDate(day, month, year);
	std::cout << this->birthday.GetDate();
	return 1;
}

bool Student::EnterIdentifier() {
	int default_position = 20;
	std::string info_alert = "Enter the student's identifier (must be unique): ";
	std::regex pattern("^\\d{2}[А-ЯЁ]\\d{4}$");
	std::string _name;
	Cursor cursor;
	cursor.SetTextColor(7);
	cursor.SetPosition(25, default_position);
	cursor.SetVisible(true, 100);
	std::cout << info_alert;
	std::cin >> _name;

	while (!std::regex_match(_name, pattern)) {
		if (_name == "exit") {
			return 0;
		}
		cursor.SetPosition(25, default_position - 2);
		cursor.SetTextColor(12);
		std::cout << "Invalid format. Try again              ";
		cursor.SetTextColor(7);
		cursor.SetPosition(25 + info_alert.size(), default_position);
		std::cout << std::string(_name.size(), ' ');
		cursor.SetPosition(25 + info_alert.size(), default_position);
		std::cin.ignore(_name.size() + 1, '\n');
		std::cin >> _name;
	}
	cursor.SetVisible(false, 100);
	strcpy_s(this->identifier, _name.c_str());
	return 1;
}

bool Student::EnterGender() {
	int default_position = 23;
	std::string info_alert = "Enter the student's sex (М/Ж): ";
	std::regex pattern("^[мжМЖmfMF]$");
	std::string _gender;
	Cursor cursor;
	cursor.SetTextColor(7);
	cursor.SetPosition(25, default_position);
	cursor.SetVisible(true, 100);
	std::cout << info_alert;
	std::cin >> _gender;

	while (!std::regex_match(_gender, pattern)) {
		if (_gender == "exit") {
			return 0;
		}
		cursor.SetPosition(25, default_position - 2);
		cursor.SetTextColor(12);
		std::cout << "Invalid format. Try again";
		cursor.SetTextColor(7);
		cursor.SetPosition(25 + info_alert.size(), default_position);
		std::cout << std::string(_gender.size(), ' ');
		cursor.SetPosition(25 + info_alert.size(), default_position);
		std::cin.ignore(_gender.size() + 1, '\n');
		std::cin >> _gender;
	}
	cursor.SetVisible(false, 100);
	if (_gender == "F" or _gender == "f" or _gender == "Ж" or _gender == "ж") this->gender = 0;
	else this->gender = 1;
	return 1;
}

bool Student::EnterGroup(std::vector<Group> _groups) {
	Cursor cursor;
	std::vector<std::string> SelectGroupOptions;
	cursor.SetPosition(25, 8);
	cursor.SetTextColor(2);
	std::cout << "Select the student's group";

	cursor.SetPosition(25, 19);
	for (int i = 0; i < _groups.size(); i++) {
		SelectGroupOptions.push_back(_groups[i].name);
	}
	BaseMenu SelectGroup(25, 10, SelectGroupOptions);
	int index = SelectGroup.run();
	this->group_id = _groups[index].id;
	return 1;
}


// Функции предмета
bool Subject::EnterName() {
	int default_position = 8;
	std::string info_alert = "Enter the subject name: ";
	std::regex pattern("^[а-яА-Я0-9\\s-]+$");
	std::string _subject;
	Cursor cursor;
	cursor.SetTextColor(7);
	cursor.SetPosition(25, default_position);
	cursor.SetVisible(true, 100);
	std::cout << info_alert;
	std::cin.ignore(63, '\n');
	std::getline(std::cin, _subject);

	while (!std::regex_match(_subject, pattern)) {
		if (_subject == "exit") {
			return 0;
		}
		cursor.SetPosition(25, default_position - 2);
		cursor.SetTextColor(12);
		std::cout << "Invalid format. Try again";
		cursor.SetTextColor(7);
		cursor.SetPosition(25 + info_alert.size(), default_position);
		std::cout << std::string(_subject.size(), ' ');
		cursor.SetPosition(25 + info_alert.size(), default_position);
		std::cin.ignore(_subject.size() + 1, '\n');
		std::cin >> _subject;
	}
	cursor.SetVisible(false, 100);
	strcpy_s(this->name, _subject.c_str());
	return 1;
}


// Функции класса успеваемости
int Grade::EnterNumber(int default_position, std::string info_alert) {
	std::string input;
	std::regex pattern(R"(^\d+$)");
	
	Cursor cursor;
	cursor.SetPosition(25, default_position);
	cursor.SetTextColor(2);
	std::cout << info_alert;
	cursor.SetTextColor(7);
	cursor.SetVisible(true, 100);
	std::cin >> input;

	while (!std::regex_match(input, pattern)) {
		if (input == "exit") {
			return 0;
		}
		cursor.SetPosition(25, default_position - 2);
		cursor.SetTextColor(12);
		std::cout << "Invalid format. Try again  ";
		cursor.SetTextColor(7);
		cursor.SetPosition(25 + info_alert.size(), default_position);
		std::cout << std::string(input.size(), ' ');
		cursor.SetPosition(25 + info_alert.size(), default_position);
		std::cin.ignore(input.size() + 1, '\n');
		std::cin >> input;
	}
	return std::stoi(input);
}

