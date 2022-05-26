#include "Enviroment.h"

Enviroment::Enviroment(unsigned int size, unsigned int num_agents, CONSTANTS::DAY_OF_THE_WEEK day)
{
	m_grid_size = size;
	m_num_agents = num_agents;
	m_day = day;
	//Points* points = new Points[m_grid_size * m_grid_size];
	int position = 0;
	for (int i = 0; i < m_grid_size; i++)
	{
		std::vector<std::shared_ptr<Points>> temp = {};
		for (int e = 0; e < m_grid_size; e++)
		{
			std::shared_ptr<Points> point(new Points);
			temp.push_back(std::move(point));
			position++;
		}
		grid.push_back(temp);
	}
}

Enviroment::~Enviroment()
{
	for (int i = 0; i < m_grid_size; i++)
	{
		grid[i].clear();
	}
}

void Enviroment::initilise_agent_location(std::vector<std::shared_ptr<Agent>> locations)
{
	for (int i = 0; i < locations.size(); i++)
	{
		unsigned int x = std::get<0>(locations[i]->get_location());
		unsigned int y = std::get<1>(locations[i]->get_location());
		grid[x][y]->agents.push_back(locations[i]);
	}
}

void Enviroment::edit_agent_location(const std::pair<unsigned int, unsigned int>& new_location, std::shared_ptr<Agent> target_agent)
{
	m_position_lock.lock();

	std::pair<unsigned int, unsigned int> location = target_agent->get_location();
	unsigned int x = location.first;
	unsigned int y = location.second;

	grid[x][y]->agents.erase(std::remove(grid[x][y]->agents.begin(), grid[x][y]->agents.end(), target_agent));

	x = new_location.first;
	y = new_location.second;

	grid[x][y]->agents.push_back(target_agent);

	target_agent->set_location(new_location);

	m_position_lock.unlock();
}

//void Enviroment::update_public_building(int count, std::unique_ptr<public_buildings>& p_building)
//{
//	std::pair<int, int> hours = p_building->get_opening_hours(m_day);
//	if ((count < hours.first || count > hours.second) && p_building->closed == false)
//	{
//		p_building->modify_closed(true);
//	}
//	else if (p_building->closed == true)
//	{
//		p_building->modify_closed(false);
//	}
//}

unsigned int Enviroment::get_size()
{
	return m_grid_size;
}

const std::shared_ptr<Enviroment::Points> Enviroment::pass_grid(unsigned int index_1, unsigned int index_2)
{
	return grid[index_1][index_2];
}

unsigned int Enviroment::get_num_agents()
{
	return m_num_agents;
}
