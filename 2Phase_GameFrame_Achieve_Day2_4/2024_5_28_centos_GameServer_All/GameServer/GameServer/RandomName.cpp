#include "RandomName.h"
#include <iostream>
#include <fstream>

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

void RandomName::GetName()
{
	static int first = true;
	for (auto single : m_name_pool)
	{
		if (first)
		{
			for (auto last_name : single->m_last_name_list)
			{
				std::cout << "名: " << last_name << std::endl;
			}
			first = false;
		}
		std::cout << "姓: " << single->m_first_name << " ";
	}
}

void RandomName::ReleaseName(std::string _name)
{
}
