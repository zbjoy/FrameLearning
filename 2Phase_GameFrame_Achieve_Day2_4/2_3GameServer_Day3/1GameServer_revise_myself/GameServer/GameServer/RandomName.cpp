#include "RandomName.h"
#include <fstream>
#include <random>

static std::default_random_engine rand_engine(time(NULL));

std::string RandomName::GetName()
{
    if (m_name_pool.size() == 0)
    {
        std::cout << "姓名池为空" << std::endl;
        return "抱歉玩家";
    }
    int firstName_index = rand_engine() % m_name_pool.size();
    int lastName_index = rand_engine() % m_name_pool[firstName_index]->last_name_list.size();

    std::string name = m_name_pool[firstName_index]->first_name + " " + m_name_pool[firstName_index]->last_name_list[lastName_index]; 
    m_name_pool[firstName_index]->last_name_list.erase(m_name_pool[firstName_index]->last_name_list.begin() + lastName_index);

    return name;
}

bool RandomName::ReleaseName(std::string _name)
{
    int space_pos = _name.find(" ", 0);
    std::string firstNameStr = _name.substr(0, space_pos);
    std::string lastNameStr = _name.substr(space_pos + 1, _name.size() - space_pos - 1);

    for (auto single : m_name_pool)
    {
        if (single->first_name == firstNameStr)
        {
            single->last_name_list.push_back(lastNameStr);
            return true;
        }
    }

    FirstName* firstName = new FirstName();
    firstName->first_name = firstNameStr;
    firstName->last_name_list.push_back(lastNameStr);
    m_name_pool.push_back(firstName);
    return true;
}

bool RandomName::LoadName(std::string _firstNameFile, std::string _lastNameFile)
{
    std::ifstream ifs_firstName(_firstNameFile);
    std::ifstream ifs_lastName(_lastNameFile);

    std::string lastNameStr;
    std::vector<std::string> tmp;
    /* 将LastName存入tmp */
    while (std::getline(ifs_lastName, lastNameStr))
    {
        tmp.push_back(lastNameStr);
    }

    /* 将FirstName带着LastName存入m_name_pool */
    std::string firstNameStr;
    while (std::getline(ifs_firstName, firstNameStr))
    {
        FirstName* firstName = new FirstName();
        firstName->first_name = firstNameStr;
        firstName->last_name_list = tmp;
        m_name_pool.push_back(firstName);
    }

    ifs_firstName.close();
    ifs_lastName.close();
    return true;
}
