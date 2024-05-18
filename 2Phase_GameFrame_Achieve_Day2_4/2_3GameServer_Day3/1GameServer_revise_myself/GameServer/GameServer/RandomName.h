#pragma once
#include <iostream>
#include <list>
#include <vector>

struct FirstName
{
	std::string first_name;
	std::vector<std::string> last_name_list;
};

class RandomName
{
	std::vector<FirstName*> m_name_pool;
public:

	std::string GetName();
	bool ReleaseName(std::string _name);
	bool LoadName(std::string _firstNameFile = "random_first.txt", std::string _lastNameFile = "random_last.txt");
};

