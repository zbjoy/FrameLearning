#pragma once
#include <iostream>
#include <string>
#include <list>
#include <vector>

struct FirstName
{
	std::string m_first_name;
	std::vector<std::string> m_last_name_list;
};

class RandomName
{
public:
	std::vector<FirstName*> m_name_pool;

	RandomName();

	std::string GetName();
	bool ReleaseName(std::string _name);

	void LoadFile(std::string firstNameFile = "./random_first.txt", std::string lastNameFile = "./random_last.txt");
};

