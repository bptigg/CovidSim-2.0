#include "Movement.h"

void Movement::update_agent(Agent* target_agent)
{
	Agent::task_state mode = target_agent->get_task_state();
	std::pair<unsigned int, unsigned int> new_location;

	switch (mode)
	{
	case Agent::task_state::IDLE:
		new_location = agent_random_walk(target_agent);
		break;
	case Agent::task_state::TRANSIT:
		//weighted walk
		break;
	case Agent::task_state::ACTIVE:
		break;
	default:
		break;
	}

	if (mode == Agent::task_state::IDLE || mode == Agent::task_state::TRANSIT)
	{
		m_world->edit_agent_location(new_location, target_agent);
	}
}

Movement::Movement(unsigned int grid, Enviroment* world)
	:m_grid_size(grid), m_world(world)
{
}

std::pair<unsigned int, unsigned int> Movement::agent_random_walk(Agent*& target_agent)
{
	Matrix<double> random_move(1, 4, 0.25);
	
	unsigned int direction = random::Discrete_distribution(random_move(0), 1, true)[0];
	std::pair<unsigned int, unsigned int> location = target_agent->get_location();
	int x = location.first;
	int y = location.second;
	switch (direction)
	{
	case 0:
		if (x >= m_grid_size-1)
		{
			x = x - 1;
			direction = 1;
			break;
		}
		x = x + 1;
		direction = 0;
		break;
	case 1:
		if (x <= 0)
		{
			x = x + 1;
			direction = 0;
			break;
		}
		x = x - 1;
		direction = 1;
		break;
	case 2:
		if (y >= m_grid_size-1)
		{
			y = y - 1;
			direction = 3;
			break;
		}
		y = y + 1;
		direction = 2;
		break;
	case 3:
		if (y <= 0)
		{
			y = y + 1;
			direction = 2;
			break;
		}
		y = y - 1;
		direction = 3;
		break;
	default:
		break;
	}

	return std::make_pair(x, y);
}
