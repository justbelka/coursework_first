#include "crypto.h"
#include <string>
#include <format>
#include <fstream>


void Crypto::Crypt(std::string filename) {
	std::string password;
	password.reserve(64);

	for (int i = 0; i < 64; i++) {
		switch (std::rand() % 3) {
		case 0:
			password += static_cast<char>(std::rand() % 10 + '0');
			break;
		case 1:
			password += static_cast<char>(std::rand() % 26 + 'A');
			break;
		case 2:
			password += static_cast<char>(std::rand() % 26 + 'a');
			break;
		}
	}
	std::string token = filename.substr(0, filename.find("."));
	token = token.substr(5, token.size());
	std::string key = token + "_key.txt";

	std::string command = std::format("openssl\\bin\\openssl.exe enc -aes-256-cbc -salt -in {} -out {}.enc -pass pass:{}", 
		filename, filename, password
		);
	system(command.c_str());
	remove(filename.c_str());

	std::ofstream file;
	file.open(key, std::ios::binary);
	file.write(password.c_str(), 65);
	file.close();

	command = std::format("openssl\\bin\\openssl.exe pkeyutl -encrypt -inkey rsa.public -pubin -in {} -out data\\{}.enc", key, key);
	system(command.c_str());
	remove(key.c_str());
}

void Crypto::Decrypt(std::string filename) {
	std::string password;
	std::string token = filename.substr(0, filename.find("."));
	token = token.substr(5, token.size());
	std::string key = token + "_key.txt";

	std::string command = std::format("openssl\\bin\\openssl.exe pkeyutl -decrypt -inkey rsa.private -in data\\{}.enc -out data\\{}", key, key);
	system(command.c_str());
	remove(std::format("data/{}.enc", key).c_str());
	std::ifstream file;
	password.resize(64);
	file.open(std::format("data/{}", key), std::ios::binary);
	file.read(password.data(), 64);
	file.close();
	remove(std::format("data/{}", key).c_str());
	command = std::format("openssl\\bin\\openssl.exe enc -aes-256-cbc -d -in {}.enc -out {} -pass pass:{}",
		filename, filename, password
		);
	system(command.c_str());
	remove(std::format("{}.enc", filename).c_str());
}
