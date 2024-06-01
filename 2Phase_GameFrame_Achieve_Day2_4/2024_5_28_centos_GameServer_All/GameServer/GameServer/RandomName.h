#pragma once
#include <vector>
#include <string>

struct FirstName
{
	std::vector<std::string> m_last_name_list;
	std::string m_first_name;
};

class RandomName
{
public:
	void Init(std::string _last_name_file = "./random_last.txt", std::string _first_name_file = "./random_first.txt");
	std::string GetName();
	void ReleaseName(std::string _name);

private:
	std::vector<FirstName*> m_name_pool;
};

