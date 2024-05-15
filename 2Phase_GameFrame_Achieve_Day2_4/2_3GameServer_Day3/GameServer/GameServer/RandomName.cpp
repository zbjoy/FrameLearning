#include "RandomName.h"
#include <iostream>
#include <fstream>
#include <random>

// static std::default_random_engine rand_engine(time(NULL));

RandomName::RandomName()
{
	LoadFile();
}

std::string RandomName::GetName()
{
	/* 取名 */
	//int num = rand_engine() % m_name_pool.size();
	//std::string first = m_name_pool[num]->m_first_name;

	//std::string last = m_name_pool[num]->m_last_name_list[rand_engine() % m_name_pool[num]->m_last_name_list.size()];

	//if (m_name_pool[num]->m_last_name_list.size() == 0)
	//{
	//	m_name_pool.erase(m_name_pool.begin() + num);
	//}
	//return first + " " + last;
}

void RandomName::Release(std::string _name)
{

}

void RandomName::LoadFile()
{
	std::ifstream ifs_first("./random_first.txt");
	std::ifstream ifs_last("./random_last.txt");

	std::string last_name;
	std::vector<std::string> last_name_list;
	while (std::getline(ifs_last, last_name))
	{
		last_name_list.push_back(last_name);
	}

	std::string first_name_str;
	while (std::getline(ifs_first, first_name_str))
	{
		FirstName* first_name = new FirstName();
		first_name->m_first_name = first_name_str;
		first_name->m_last_name_list = last_name_list;
		m_name_pool.push_back(first_name);
	}

	ifs_first.close();
	ifs_last.close();
}

RandomName::~RandomName()
{
}
