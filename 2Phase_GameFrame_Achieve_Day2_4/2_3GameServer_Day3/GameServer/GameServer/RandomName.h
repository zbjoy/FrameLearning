#pragma once
#include <vector>
#include <random>

class FirstName
{
public:
	std::string m_first_name;
	std::vector<std::string> m_last_name_list;
};

class RandomName
{
public:
	RandomName();
	std::string GetName();
	void Release(std::string _name);
	//void test();
	void LoadFile();
	~RandomName();

private:
	std::vector<FirstName*> m_name_pool;
};

