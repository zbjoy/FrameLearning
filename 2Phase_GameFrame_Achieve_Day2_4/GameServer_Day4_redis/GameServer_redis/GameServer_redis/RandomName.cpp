#include "RandomName.h"
#include <fstream>
#include <random>

static std::default_random_engine rand_engine(time(NULL));

RandomName::RandomName()
{
}

std::string RandomName::GetName()
{
	int first_name_index = rand_engine() % m_name_pool.size();
	int last_name_index = rand_engine() % m_name_pool[first_name_index]->m_last_name_list.size();

	std::string temp = m_name_pool[first_name_index]->m_first_name + " " + m_name_pool[first_name_index]->m_last_name_list[last_name_index];

	m_name_pool[first_name_index]->m_last_name_list.erase(m_name_pool[first_name_index]->m_last_name_list.begin() + last_name_index);

	return temp;
}

bool RandomName::ReleaseName(std::string _name)
{
	int space_pos = _name.find(" ", 0);
	std::string first_name = _name.substr(0, space_pos);
	std::string last_name = _name.substr(space_pos + 1, _name.size() - space_pos - 1);

	for (auto first : m_name_pool)
	{
		if (first->m_first_name == first_name)
		{
			first->m_last_name_list.push_back(last_name);
			return true;
		}
	}

	FirstName* temp = new FirstName;
	temp->m_first_name = first_name;
	temp->m_last_name_list.push_back(last_name);
	m_name_pool.push_back(temp);
	return true;
}

void RandomName::LoadFile(std::string firstNameFile, std::string lastNameFile)
{
	std::ifstream ifs_firstName(firstNameFile);
	std::ifstream ifs_lastName(lastNameFile);

	std::vector<std::string> temp;
	std::string buf;
	while (std::getline(ifs_lastName, buf))
	{
		temp.push_back(buf);
	}

	while (std::getline(ifs_firstName, buf))
	{
		FirstName* first = new FirstName;
		first->m_last_name_list = temp;
		first->m_first_name = buf;

		m_name_pool.push_back(first);
	}

	ifs_firstName.close();
	ifs_lastName.close();
}
