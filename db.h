#pragma once
#include "cursor.h"
#include "models.h"
#include <string>
#include <vector>


class Database {
private:
	Cursor cursor;
public:
	void run(std::string error = "NOERROR");
	void action(int action);

	void CryptDatabase(int encrypt_mode = 0);
	template <typename T>
	std::vector<T> GetObjects(const std::string& filename);
	void SortStudents(std::vector<std::pair<int, double>>& score_table);
	void ViewGroups();
	void ViewGroup(Group& _group);
	void ViewGroupMarks(Group& _group);
	void ViewStudent(Student& _student, Group& _group); 
	void CreateGroup();
	void CreateStudent();
	void CreateSubject();
	void CreateGrade(Student& _student, Group& _group);
	void EditStudent(Student& _student, Group& _group);
	void DeleteStudent(Student& _student, Group& _group);
};
