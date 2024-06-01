#include "RandomName.h"
#include <iostream>
#include <fstream>
#include <random>

static std::default_random_engine rand_engine(time(NULL));

void RandomName::Init(std::string _last_name_file, std::string _first_name_file)
{
	/* 将姓名从文件中读入 */
	//std::ifstream ifs_last("./random_last.txt");
	//std::ifstream ifs_first("./random_first.txt");

	std::ifstream ifs_last(_last_name_file);
	std::ifstream ifs_first(_first_name_file);

	std::vector<std::string> last_name_list;
	std::string tempName;

	while (std::getline(ifs_last, tempName))
	{
		last_name_list.push_back(tempName);
	}

	while (std::getline(ifs_first, tempName))
	{
		FirstName* firstName = new FirstName;
		firstName->m_first_name = tempName;
		firstName->m_last_name_list = last_name_list;

		m_name_pool.push_back(firstName);
	}
}

std::string RandomName::GetName()
{
	//static int first = true;
	//for (auto single : m_name_pool)
	//{
	//	if (first)
	//	{
	//		for (auto last_name : single->m_last_name_list)
	//		{
	//			std::cout << "名: " << last_name << std::endl;
	//		}
	//		first = false;
	//	}
	//	std::cout << "姓: " << single->m_first_name << " ";
	//}

	int first_name_index = rand_engine() % m_name_pool.size();
	int last_name_index = rand_engine() % m_name_pool[first_name_index]->m_last_name_list.size();

	std::string first_name = m_name_pool[first_name_index]->m_first_name;
	std::string last_name = m_name_pool[first_name_index]->m_last_name_list[last_name_index];

	auto tempPointer = m_name_pool[first_name_index]->m_last_name_list.erase(m_name_pool[first_name_index]->m_last_name_list.begin() + last_name_index);

	if (m_name_pool[first_name_index]->m_last_name_list.size() == 0)
	{
		auto pointer = m_name_pool.erase(m_name_pool.begin() + first_name_index);
		/* 取出后删除开辟的空间 */
		delete *pointer;
	}

	return first_name + " " + last_name;
	
}

void RandomName::ReleaseName(std::string _name)
{
	int pos = _name.find(" ");
	std::string first_name = _name.substr(0, pos);
	std::string last_name = _name.substr(pos + 1, _name.size() - pos - 1);

	// std::cout << "pos: " << pos << std::endl;
	// std::cout << "first_name:" << first_name << std::endl;
	// std::cout << "last_name:" << last_name << std::endl;

	bool isFind = false;
	for (auto single : m_name_pool)
	{
		if (single->m_first_name == first_name)
		{
			std::cout << "(ReleaseName)将 lastName 为: " << last_name << " 的加入" << single->m_first_name << " 中 " << std::endl;
			single->m_last_name_list.push_back(last_name);
			isFind = true;
		}
	}

	if (!isFind)
	{
		std::cout << "ReleaseName::isFind: " << std::endl;
		FirstName* firstName = new FirstName;
		firstName->m_first_name = first_name;
		firstName->m_last_name_list.push_back(last_name);
	}

	return;
	
}
