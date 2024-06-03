#include "db.h"

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	Database db;
	db.CryptDatabase(1);
	db.run();
	db.CryptDatabase();

	return 0;
}