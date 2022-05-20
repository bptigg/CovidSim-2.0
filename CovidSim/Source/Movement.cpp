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
		new_location = agent_weighted_walk(target_agent);
		//transit requires other stuff like A* and planning
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
	random_move = generate_move_matrix(target_agent, true);
	
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

std::pair<unsigned int, unsigned int> Movement::agent_weighted_walk(Agent*& target_agent)
{
	Matrix<double> weighted_move(1, 4, 0);
	weighted_move = generate_move_matrix(target_agent);

	unsigned int direction = random::Discrete_distribution(weighted_move(0), 1, true)[0];
	std::pair<unsigned int, unsigned int> location = target_agent->get_location();
	int x = location.first;
	int y = location.second;
	switch (direction)
	{
	case 0:
		if (x >= m_grid_size - 1)
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
		if (y >= m_grid_size - 1)
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

Matrix<double> Movement::generate_move_matrix(Agent* target_agent, bool random)
{
	Matrix<double> move_matrix(1, 4, 0);
	if (random == true)
	{
		std::vector<int> random_numbers = {};
		int normalize = 0;
		for (int i = 0; i < 4; i++)
		{
			random_numbers.push_back(move_matrix(0, i) = random::Random_number(0, 100, random_numbers));
			normalize = normalize + random_numbers[i];
		}
		for (int i = 0; i < 4; i++)
		{
			move_matrix(0, i) = (double)random_numbers[i] / normalize;
		}
	}
	else
	{
		int delta_x = target_agent->get_target_location().first - target_agent->get_location().first;
		int delta_y = target_agent->get_target_location().second - target_agent->get_location().second;

		bool x_ve = true, y_ve = true;

		if (delta_x > 0)
		{
			x_ve = false;
		}
		if (delta_y > 0)
		{
			y_ve = false;
		}

		delta_x = abs(delta_x);
		delta_y = abs(delta_y);

		double y_scaler = 0;
		double x_scaler = 0;
		bool create_scaler = true;

		if (delta_x == 0)
		{
			x_scaler = 0.2;
			create_scaler = false;

		}
		if (delta_y == 0)
		{
			y_scaler = 0.2;
			create_scaler = false;
		}

		if (create_scaler == true)
		{
			if (delta_x >= delta_y)
			{
				x_scaler = ((double)delta_y / (double)delta_x) / 2;
				y_scaler = 1 - x_scaler;
			}
			else
			{
				y_scaler = ((double)delta_x / (double)delta_y) / 2;
				x_scaler = 1 - y_scaler;
			}
		}

		double x_norm = (double)delta_x / m_grid_size;
		double y_norm = (double)delta_y / m_grid_size;

		double xopp_norm = 1 - x_norm;
		double yopp_norm = 1 - y_norm;

		std::vector<double> weights = { x_norm * x_scaler, xopp_norm * x_scaler, y_norm * y_scaler, yopp_norm * y_scaler };

		if (x_ve == true)
		{
			move_matrix(0, 0) = weights[0];
			move_matrix(0, 1) = weights[1];
		}
		else
		{
			move_matrix(0, 0) = weights[1];
			move_matrix(0, 1) = weights[0];
		}

		if (y_ve == true)
		{
			move_matrix(0, 2) = weights[2];
			move_matrix(0, 3) = weights[3];
		}
		else
		{
			move_matrix(0, 3) = weights[3];
			move_matrix(0, 2) = weights[2];
		}
	}
	return move_matrix;
}
