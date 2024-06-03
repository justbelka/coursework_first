#pragma once
#include <string>
#include <vector>


class FSManager {
public:
	std::vector<std::string> ListDirectory(std::string path);
	bool IsExist(std::string path);
};
