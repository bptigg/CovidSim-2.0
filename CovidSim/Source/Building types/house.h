#pragma once
#include "../Building.h"
class house : public base_building
{
protected:
	std::vector<std::shared_ptr<Agent>> m_family_group;  

	std::mutex m_family_lock;

public:
	house(std::vector<std::shared_ptr<Agent>>& m_family_group);
	~house();

	std::vector<std::shared_ptr<Agent>> get_family_group();

};

