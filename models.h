#pragma once
#include "date.h"
#include <string>
#include <vector>


class Group {
public:
	unsigned int id;
	char name[11];

	bool EnterName();
};


class Student {
public:
	bool gender = true;
	Date birthday;
	unsigned int id;
	unsigned int group_id;
	char identifier[31] = "";
	char surname[31] = "";
	char name[31] = "";
	char patronymic[31] = "";

	bool EnterSurname();
	bool EnterName();
	bool EnterPatronymic();
	bool EnterBirthday();
	bool EnterIdentifier();
	bool EnterGroup(std::vector<Group> _groups);
	bool EnterGender();
};


class Subject {
public:
	unsigned int id;
	char name[63];

	bool EnterName();
};


class Mark {
public:
	unsigned int id;
	unsigned int subject_id;
	unsigned short int mark;
};


class Grade {
public:
	unsigned int student_id;
	unsigned short int marks[11][10];

	int EnterNumber(int default_position, std::string info_alert);
};
