#include <regex>
#include <format>
#include <iomanip>
#include <unordered_set>
#include <unordered_map>
#include "db.h"
#include "menu.h"
#include "fsmanager.h"
#include "config.h"
#include "crypto.h"


// Реализована полностью
void Database::run(std::string error) {
	cursor.SetConsoleName("MIREA Students DB");
	cursor.ClearConsole();
	cursor.SetTextColor(10);
	std::cout << WELCOME;
	cursor.SetPosition(0, 28);
	std::cout << MADE_BY;

	if (error != "NOERROR") {
		cursor.SetPosition(25, 7);
		cursor.SetTextColor(12);
		std::cout << error;
		cursor.SetTextColor(7);
	}

	std::vector<std::string> StartMenuOptions = {
		"View created groups",
		"Create new group",
		"Create new student",
		"Create new subject"
	};
	BaseMenu StartMenu(25, 12, StartMenuOptions);
	int event = StartMenu.run();
	if (event != 9999) Database::action(event);
	else return;
}

// Реализована полностью
void Database::action(int action) {
	switch (action) {
	case 0:
		ViewGroups();
		break;
	case 1:
		CreateGroup();
		break;
	case 2:
		CreateStudent();
		break;
	case 3:
		CreateSubject();
	}
}

// Реализована полностью
void Database::CryptDatabase(int encrypt_mode) {
	std::vector<std::string> datafiles = {
		"data/students.bin"
	};
	Crypto crypter;
	for (int i = 0; i < datafiles.size(); i++) {
		if (encrypt_mode == 0) crypter.Crypt(datafiles[i]);
		else crypter.Decrypt(datafiles[i]);
	}
}

// Реализована полностью
template <typename T>
std::vector<T> Database::GetObjects(const std::string& filename) {
	FSManager Explorer;
	std::vector<T> objects;
	if (Explorer.IsExist(filename)) {
		FILE* binaryfile;
		if (fopen_s(&binaryfile, filename.c_str(), "rb") == 0) {
			fseek(binaryfile, 0, SEEK_END);
			long fileSize = ftell(binaryfile);
			rewind(binaryfile);
			size_t numRecords = fileSize / sizeof(T);
			objects.resize(numRecords);
			fread(objects.data(), sizeof(T), numRecords, binaryfile);
			fclose(binaryfile);
		}
	}
	return objects;
}

// Реализована полностью
void Database::SortStudents(std::vector<std::pair<int, double>>& score_table) {
	bool swapped = true;
	int n = score_table.size();
	while (swapped) {
		swapped = false;
		for (int i = 0; i < n - 1; i++) {
			if (score_table[i].second < score_table[i + 1].second) {
				std::swap(score_table[i], score_table[i + 1]);
				swapped = true;
			}
		}
		n--;
	}
}

// Реализована полностью
void Database::ViewGroups() {
	cursor.ClearConsole();
	cursor.SetPosition(25, 8);
	cursor.SetTextColor(2);
	std::cout << "All MIREA groups";
	cursor.SetPosition(25, 19);
	std::vector<std::string> GroupsMenuOptions;
	std::vector<Group> Groups = Database::GetObjects<Group>("data/groups.bin");

	if (Groups.size() == 0) {
		Database::run("No groups found! Try create new group!\n");
	}
	else {
		for (int i = 0; i < Groups.size(); i++) {
			GroupsMenuOptions.push_back(Groups[i].name);
		}
		BaseMenu GroupsMenu(25, 10, GroupsMenuOptions);
		int index = GroupsMenu.run(1);
		if (index == 1000) Database::run();
		else Database::ViewGroup(Groups[index]);
	}
}

// Реализована полностью
void Database::ViewGroup(Group& _group) {
	cursor.ClearConsole();
	cursor.SetPosition(25, 8);
	cursor.SetTextColor(2);
	std::cout << "The group " << _group.name;
	cursor.SetPosition(25, 19);
	std::vector<std::string> StudentsMenuOptions = {
		"View the academic performance of the group"
	};
	std::vector<Student> students = Database::GetObjects<Student>("data/students.bin");
	std::vector<Student> group_students;

	if (students.size() == 0) {
		Database::run("No students in database!\n");
	}
	else {
		for (int i = 0; i < students.size(); i++) {
			if (students[i].group_id == _group.id) {
				StudentsMenuOptions.push_back(std::format(
					"{} - {} {} {}",
					students[i].identifier,
					students[i].surname,
					students[i].name,
					students[i].patronymic
				));
				group_students.push_back(students[i]);
			}
		}
		if (StudentsMenuOptions.size() == 1) {
			Database::run("No students in group! Try add new!\n");
		}
		else {
			BaseMenu GroupsMenu(25, 10, StudentsMenuOptions);
			int index = GroupsMenu.run(1);
			if (index == 1000) Database::ViewGroups();
			else if (index == 0) Database::ViewGroupMarks(_group);
			else Database::ViewStudent(group_students[index - 1], _group);
		}
	}
}

// Реализована полностью
void Database::ViewGroupMarks(Group& _group) {
	std::unordered_map<int, int> mark_values;
	std::unordered_map<int, Student> student_table;
	std::unordered_set<int> stud_ids;
	std::vector<std::pair<int, double>> score_table;
	std::vector<Mark> marks = Database::GetObjects<Mark>("data/marks.bin");
	std::vector<Student> students = Database::GetObjects<Student>("data/students.bin");
	std::vector<Grade> grades = Database::GetObjects<Grade>("data/grades.bin");

	// Формируем таблицу (id оценки - значение)
	for (int i = 0; i < marks.size(); i++) {
		mark_values[marks[i].id] = marks[i].mark;
	}
	mark_values[52428] = 0;

	// Меню, чтобы выбрать семестр для сортировки группы
	cursor.ClearConsole();
	cursor.SetPosition(12, 3);
	cursor.SetTextColor(10);
	std::cout << "Select a semester number:";
	HorizontalMenu semesteres(10, 5, std::vector<std::string>{
		" | 1 | ", " | 2 | ", " | 3 | ", " | 4 | ", " | 5 | ", " | 6 | ", " | 7 | ", " | 8 | ", " | 9 | ",
			" | 10 | ", " | 11 | "
	});
	cursor.SetPosition(12, 7);
	std::cout << "Select a birth year:";
	HorizontalMenu years(10, 9, std::vector<std::string>{
		" | 2003 | ", " | 2004 | ", " | 2005 | ", " | 2006 | "
	});
	int semester = semesteres.run(1);
	if (semester == 1000) {
		Database::ViewGroup(_group);
		return;
	}
	int year = 2003 + years.run();

	students.erase(std::remove_if(students.begin(), students.end(), [&_group, &year](const Student& student) {
		return student.group_id != _group.id or student.birthday.year != year;
		}),
		students.end());
	for (int i = 0; i < students.size(); i++) { 
		stud_ids.insert(students[i].id);
		student_table[students[i].id] = students[i];
	}
	// Удаляем из всех выбранных объектов успеваемостей студентов лишние записи
	grades.erase(std::remove_if(grades.begin(), grades.end(), [&stud_ids](const Grade& gr) {
		return stud_ids.find(gr.student_id) == stud_ids.end();
		}),
		grades.end());

	for (int i = 0; i < students.size(); i++) {
		double score = 0;
		for (int j = 0; j < 10; j++) score += mark_values[grades[i].marks[semester][j]];
		score_table.push_back({ students[i].id, score / 10 });
	}

	Database::SortStudents(score_table);

	std::vector<std::string> options;
	for (int i = 0; i < score_table.size(); i++) {
		int id = score_table[i].first;
		Student& student = student_table[id];
		std::string fio = std::format("{} {}.{}.", student.surname, student.name[0], student.patronymic[0]);
		fio.resize(21, ' ');
		options.push_back(std::format("| {}| {:.1f} |", fio, score_table[i].second));
	}

	// отрисовываем табличку с результатами
	cursor.SetPosition(10, 11);
	std::cout << "GROUP RESULTS:" << std::endl;
	cursor.SetPosition(10, 12);
	std::cout << "+----------------------+-----+";
	if (students.size() >= 15) cursor.SetPosition(10, 28);
	else cursor.SetPosition(10, 13 + students.size());
	std::cout << "+----------------------+-----+";
	ListingMenu menu(10, 13, options);
	int index = menu.run(1);
	if (index == 1000) Database::ViewGroup(_group);
	else Database::ViewStudent(student_table[score_table[index].first], _group);
}

// Реализована полностью
void Database::ViewStudent(Student& _student, Group& _group) {
	cursor.ClearConsole();
	cursor.SetPosition(1, 1);
	cursor.SetTextColor(2);
	std::cout << "Group: " << _group.name << std::endl;
	cursor.SetTextColor(12);
	std::cout << " ID: " << _student.identifier << std::endl;
	cursor.SetTextColor(7);
	char sex = 'М';
	if (!_student.gender) sex = 'Ж';
	std::cout << "+---------------+-----------------------+" << std::endl;
	std::cout << "| Surname       | " << std::setw(22) << std::left << _student.surname << "|" << std::endl;
	std::cout << "| Name          | " << std::setw(22) << std::left << _student.name << "|" << std::endl;
	std::cout << "| Patronymic    | " << std::setw(22) << std::left << _student.patronymic << "|" << std::endl;
	std::cout << "+---------------+-----------------------+" << std::endl;
	std::cout << "| Date of birth | " << std::setw(22) << std::left << _student.birthday.GetDate() << "|" << std::endl;
	std::cout << "+---------------+-----------------------+" << std::endl;
	std::cout << "| Sex           | " << std::setw(22) << std::left << sex << "|" << std::endl;
	std::cout << "+---------------+-----------------------+" << std::endl << std::endl;

	cursor.SetTextColor(13);
	int current_grade = 0;
	std::vector<Grade> grades = Database::GetObjects<Grade>("data/grades.bin");
	std::vector<Mark> marks = Database::GetObjects<Mark>("data/marks.bin");
	unsigned short int marks_list[11][10];
	for (int i = 0; i < grades.size(); i++) {
		if (grades[i].student_id == _student.id) {
			current_grade = i;
			break;
		}
	}
	for (int i = 0; i < 11; i++) {
		for (int j = 0; j < 10; j++) {
			for (int k = 0; k < marks.size(); k++) {
				if (grades[current_grade].marks[i][j] == marks[k].id) {
					marks_list[i][j] = marks[k].mark;
					break;
				}
			}
		}
	}

	std::cout << "                  S U B J E C T S" << std::endl;
	std::cout << "  +-------------------------------------------+" << std::endl;
	std::cout << "  | № | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10|" << std::endl;
	std::cout << "  +-------------------------------------------+" << std::endl;
	std::cout << "  | 1 |" << std::endl;
	std::cout << "S | 2 |" << std::endl;
	std::cout << "E | 3 |" << std::endl;
	std::cout << "M | 4 |" << std::endl;
	std::cout << "E | 5 |" << std::endl;
	std::cout << "S | 6 |" << std::endl;
	std::cout << "T | 7 |" << std::endl;
	std::cout << "E | 8 |" << std::endl;
	std::cout << "R | 9 |" << std::endl;
	std::cout << "  | 10|" << std::endl;
	std::cout << "  | 11|" << std::endl;
	std::cout << "  +-------------------------------------------+" << std::endl;

	for (int i = 0; i < 11; i++) {
		cursor.SetPosition(7, i + 17);
		for (int j = 0; j < 10; j++) {
			if (marks_list[i][j] == 52428) {
				cursor.SetTextColor(7);
				std::cout << " * ";
				cursor.SetTextColor(13);
				std::cout << "|";
			}
			else {
				if (marks_list[i][j] == 2) cursor.SetTextColor(4);
				else if (marks_list[i][j] == 3) cursor.SetTextColor(6);
				else if (marks_list[i][j] == 4) cursor.SetTextColor(2);
				else if (marks_list[i][j] == 5) cursor.SetTextColor(10);
				std::cout << " " << marks_list[i][j];
				cursor.SetTextColor(13);
				std::cout << " |";
			}
		}
		std::cout << std::endl;
	}

	std::vector<std::string> opts = { " | Give a mark | ", " | Edit student | ", " | Delete student | ", " | Exit | "};
	HorizontalMenu menu(50, 20, opts);
	int index = menu.run();
	if (index == 0) Database::CreateGrade(_student, _group);
	else if (index == 1) Database::EditStudent(_student, _group);
	else if (index == 2) Database::DeleteStudent(_student, _group);
	else if (index == 3) Database::ViewGroup(_group);
}

// Реализована полностью
void Database::CreateGroup() {
	cursor.ClearConsole();
	cursor.SetPosition(25, 7);
	cursor.SetTextColor(2);
	std::cout << "Creation of new students group (Press \"exit\" to go to the main menu)" << std::endl;

	Group newgroup;
	if (!newgroup.EnterName()) {
		Database::run();
		return;
	}
	int last_id = 0;
	std::vector<Group> Groups = Database::GetObjects<Group>("data/groups.bin");
	if (Groups.size() != 0) last_id = Groups.back().id;

	while (true) {
		bool already_exists = false;
		for (const auto& group : Groups) {
			if (std::string(group.name) == std::string(newgroup.name)) {
				already_exists = true;
				break;
			}
		}
		if (already_exists) {
			cursor.ClearConsole();
			cursor.SetPosition(25, 6);
			cursor.SetTextColor(12);
			std::cout << "This group already exists";
			cursor.SetPosition(25, 7);
			cursor.SetTextColor(2);
			std::cout << "Creation of new students group (Press \"exit\" to go to the main menu)" << std::endl;
			if (!newgroup.EnterName()) {
				Database::run();
				return;
			}
		} else break;
	}

	newgroup.id = last_id + 1;
	FILE* binaryfile;
	fopen_s(&binaryfile, "data/groups.bin", "ab");
	fwrite(&newgroup, sizeof(Group), 1, binaryfile);
	fclose(binaryfile);

	cursor.Alert(std::format("The group {} was successfully created! (Click Enter to continue)", newgroup.name));
	Database::run();
	return;
}

// Реализована полностью
void Database::CreateStudent() {
	if (Database::GetObjects<Group>("data/groups.bin").size() == 0) {
		Database::run("You cannot create a student until no groups have been created");
		return;
	}
	cursor.ClearConsole();
	cursor.SetPosition(25, 7);
	cursor.SetTextColor(2);
	std::cout << "Creation a new student account (Press \"exit\" to go to the main menu)" << std::endl;

	int last_id = 0;
	Student student;
	std::vector<Student> all_students = Database::GetObjects<Student>("data/students.bin");
	if (!student.EnterSurname()) { Database::run(); return; }
	if (!student.EnterName()) { Database::run(); return; }
	if (!student.EnterPatronymic()) { Database::run(); return; }

	if (!student.EnterBirthday()) { Database::run(); return; }
	while (!student.birthday.CheckDate()) {
		cursor.SetPosition(25, 15);
		cursor.SetTextColor(12);
		std::cout << "Impossible date. Try again";
		std::cin.ignore(11, '\n');
		cursor.SetPosition(73, 17);
		std::cout << std::string(10, ' ');
		if (!student.EnterBirthday()) {
			Database::run();
			return;
		}
	}

	if (!student.EnterIdentifier()) { Database::run(); return; }
	while (true) {
		bool already_exists = false;
		for (const auto& stud : all_students) {
			if (std::string(stud.identifier) == std::string(student.identifier)) {
				already_exists = true;
				break;
			}
		}
		if (already_exists) {
			cursor.SetPosition(25, 18);
			cursor.SetTextColor(12);
			std::cout << "This student already exists in database";
			cursor.SetPosition(25 + 49, 20);
			std::cout << std::string(7, ' ');
			std::cin.ignore(8, '\n');
			cursor.SetPosition(25 + 49, 20);
			if (!student.EnterIdentifier()) { Database::run(); return; }
		} else break;
	}
	if (!student.EnterGender()) { Database::run(); return; }

	cursor.ClearConsole();
	if (!student.EnterGroup(Database::GetObjects<Group>("data/groups.bin"))) { Database::run(); return; }
	int count = std::count_if(all_students.begin(), all_students.end(), [&student](const Student& stud) {
		return stud.group_id == student.group_id;
		});
	while (count >= 30) {
		cursor.SetPosition(25, 6);
		cursor.SetTextColor(6);
		std::cout << "Sorry, the group is full. Choose another one";
		if (!student.EnterGroup(Database::GetObjects<Group>("data/groups.bin"))) {
			Database::run();
			return;
		}
		count = std::count_if(all_students.begin(), all_students.end(), [&student](const Student& stud) {
			return stud.group_id == student.group_id;
			});
	}

	if (all_students.size() != 0) last_id = all_students.back().id;
	student.id = last_id + 1;

	Grade grade;
	grade.student_id = student.id;

	FILE* binaryfile;
	fopen_s(&binaryfile, "data/students.bin", "ab");
	fwrite(&student, sizeof(Student), 1, binaryfile);
	fclose(binaryfile);

	fopen_s(&binaryfile, "data/grades.bin", "ab");
	fwrite(&grade, sizeof(Grade), 1, binaryfile);
	fclose(binaryfile);

	cursor.Alert(std::format("The student {} was successfully created!", student.identifier));
	Database::run();
}

// Реализована полностью
void Database::CreateSubject() {
	cursor.ClearConsole();
	cursor.SetPosition(25, 7);
	cursor.SetTextColor(2);
	std::cout << "Creation a new subject (Press \"exit\" to go to the main menu)" << std::endl;

	int last_id = 0;
	Subject subject;
	std::vector<Subject> all_subjects = Database::GetObjects<Subject>("data/subjects.bin");
	if (!subject.EnterName()) { Database::run(); return; }

	if (all_subjects.size() != 0) last_id = all_subjects.back().id;
	subject.id = last_id + 1;

	FILE* binaryfile;
	fopen_s(&binaryfile, "data/subjects.bin", "ab");
	fwrite(&subject, sizeof(Subject), 1, binaryfile);
	fclose(binaryfile);

	last_id = 0;
	std::vector<Mark> all_marks = Database::GetObjects<Mark>("data/marks.bin");

	if (all_marks.size() != 0) last_id = all_marks.back().id;
	Mark mark;
	mark.subject_id = subject.id;
	for (int i = 2; i < 6; i++) {
		last_id++;
		mark.id = last_id;
		mark.mark = i;
		all_marks.push_back(mark); 
	}

	fopen_s(&binaryfile, "data/marks.bin", "wb");
	fwrite(all_marks.data(), sizeof(Mark), all_marks.size(), binaryfile);
	fclose(binaryfile);

	cursor.Alert(std::format("New subject: {}!", subject.name));
	Database::run();
}

// Реализована полностью
void Database::CreateGrade(Student& _student, Group& _group) {
	std::vector<Subject> subjects = Database::GetObjects<Subject>("data/subjects.bin");
	if (subjects.size() != 0) {
		cursor.ClearConsole();
		std::vector<std::string> options;
		std::vector<std::string> mark_options = { "5 (Excellent)", "4 (Good)", "3 (Satisfactory)", "2 (Not satisfactory)" };
		std::vector<Mark> marks = Database::GetObjects<Mark>("data/marks.bin");
		std::vector<Grade> grades = Database::GetObjects<Grade>("data/grades.bin");
		
		int current;
		for (int i = 0; i < grades.size(); i++) {
			if (grades[i].student_id == _student.id) {
				current = i;
				break;
			}
		}
		Grade& current_grade = grades[current];

		std::string info_alert = "Enter the semester number (1-11) or \"exit\": ";
		int sem_num = current_grade.EnterNumber(8, info_alert);
		while (sem_num < 1 or sem_num > 11) {
			if (sem_num == 0) {
				Database::ViewStudent(_student, _group);
				return;
			}
			else {
				cursor.SetPosition(25, 6);
				cursor.SetTextColor(12);
				std::cout << "Incorrect number. Try again";
				cursor.SetPosition(25 + info_alert.size(), 8);
				std::cout << std::string(10, ' ');
				cursor.SetPosition(25 + info_alert.size(), 8);
				std::cin.ignore(4, '\n');
				sem_num = current_grade.EnterNumber(8, info_alert);
			}
		}
		info_alert = "Enter the subject number (1-10) or \"exit\": ";
		int sub_num = current_grade.EnterNumber(11, info_alert);
		while (sub_num < 1 or sub_num > 10) {
			if (sub_num == 0) {
				Database::ViewStudent(_student, _group);
				return;
			}
			else {
				cursor.SetPosition(25, 9);
				cursor.SetTextColor(12);
				std::cout << "Incorrect number. Try again";
				cursor.SetPosition(25 + info_alert.size(), 11);
				std::cout << std::string(10, ' ');
				cursor.SetPosition(25 + info_alert.size(), 11);
				std::cin.ignore(4, '\n');
				sub_num = current_grade.EnterNumber(11, info_alert);
			}
		}

		cursor.ClearConsole();
		cursor.SetPosition(25, 8);
		cursor.SetTextColor(2);
		std::cout << "Select subject:";
		for (int i = 0; i < subjects.size(); i++) {
			options.push_back(subjects[i].name);
		}
		BaseMenu menu(25, 10, options);
		int index = menu.run();
		
		cursor.ClearConsole();
		cursor.SetPosition(25, 8);
		cursor.SetTextColor(2);
		std::cout << "Select Mark:";
		BaseMenu MarkMenu(25, 10, mark_options);
		int mark = 5 - MarkMenu.run();

		for (int i = 0; i < marks.size(); i++) {
			if ((marks[i].mark == mark) and (marks[i].subject_id == subjects[index].id)) {
				current_grade.marks[sem_num - 1][sub_num - 1] = marks[i].id;
				break;
			}
		}
		FILE* binaryfile;
		fopen_s(&binaryfile, "data/grades.bin", "wb");
		fwrite(grades.data(), sizeof(Grade), grades.size(), binaryfile);
		fclose(binaryfile);
		Database::ViewStudent(_student, _group);
	}
	else {
		Database::run("You haven't any subjects. Try create new!");
	}	
}

// Реализована полностью
void Database::EditStudent(Student& _student, Group& _group) {
	cursor.ClearConsole();
	cursor.SetPosition(25, 7);
	cursor.SetTextColor(2);
	std::cout << "Editing a student (Press \"exit\" to go back)" << std::endl;

	std::vector<Student> all_students = Database::GetObjects<Student>("data/students.bin");
	int index;
	for (int i = 0; i < all_students.size(); i++) {
		if (all_students[i].id == _student.id) {
			index = i;
			break;
		}
	}
	Student& student = all_students[index];

	if (!student.EnterSurname()) Database::ViewStudent(_student, _group);
	if (!student.EnterName()) Database::ViewStudent(_student, _group);
	if (!student.EnterPatronymic()) Database::ViewStudent(_student, _group);

	if (!student.EnterBirthday()) Database::ViewStudent(_student, _group);
	while (!student.birthday.CheckDate()) {
		cursor.SetPosition(25, 15);
		cursor.SetTextColor(12);
		std::cout << "Impossible date. Try again";
		std::cin.ignore(11, '\n');
		cursor.SetPosition(73, 17);
		std::cout << std::string(10, ' ');
		if (!student.EnterBirthday()) {
			Database::ViewStudent(_student, _group);
			return;
		}
	}

	cursor.SetPosition(25, 20);
	cursor.SetTextColor(6);
	std::cout << "Unfortunately, you cannot change unique identifiers that are assigned only once";
	if (!student.EnterGender()) Database::ViewStudent(_student, _group);


	FILE* binaryfile;
	fopen_s(&binaryfile, "data/students.bin", "wb");
	fwrite(all_students.data(), sizeof(Student), all_students.size(), binaryfile);
	fclose(binaryfile);

	cursor.Alert(std::format("The student {} was successfully updated!", student.identifier));
	Database::ViewStudent(student, _group);
}

// Реализована полностью
void Database::DeleteStudent(Student& _student, Group& _group) {
	cursor.ClearConsole();
	cursor.SetPosition(18, 8);
	cursor.SetTextColor(6);
	std::cout << "Are you sure you want to remove the student from the database? All information will be deleted";
	HorizontalMenu AreYouSure(50, 10, std::vector<std::string>{ " | No | ", " | Yes | " });
	int index = AreYouSure.run();
	if (index == 0) Database::ViewStudent(_student, _group);
	else {
		std::vector<Student> students = Database::GetObjects<Student>("data/students.bin");
		std::vector<Grade> grades = Database::GetObjects<Grade>("data/grades.bin");
		students.erase(std::remove_if(students.begin(), students.end(), [&_student](const Student& obj) {
			return obj.id == _student.id;
			}), students.end());
		grades.erase(std::remove_if(grades.begin(), grades.end(), [&_student](const Grade& obj) {
			return obj.student_id == _student.id;
			}), grades.end());
		FILE* binaryfile;
		fopen_s(&binaryfile, "data/students.bin", "wb");
		fwrite(students.data(), sizeof(Student), students.size(), binaryfile);
		fclose(binaryfile);
		fopen_s(&binaryfile, "data/grades.bin", "wb");
		fwrite(grades.data(), sizeof(Grade), grades.size(), binaryfile);
		fclose(binaryfile);
		Database::ViewGroup(_group);
	}
}
