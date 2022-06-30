#include "Movement.h"

void Movement::update_agent(std::shared_ptr<Agent> target_agent)
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

void Movement::a_star(node& start_node, node& end_node)
{
	Matrix<double> closed_list(m_transport_matrix->get_row_size(), m_transport_matrix->get_col_size(), 0.0);
	std::vector<node> open_list;

	start_node.g_cost = 0;
	get_f_cost(start_node, end_node);
	open_list.push_back(start_node);

	bool path_found = false;

	while (!open_list.empty())
	{
		quicksort(open_list, 0, open_list.size() - 1);
		
		for (int i = 0; i < m_transport_matrix->get_row_size(); i++)
		{
			if (i == open_list.begin()->current_node_num) { continue; }
			int g_cost = (*m_transport_matrix)(open_list.begin()->current_node_num, i);
			if (g_cost != 0)
			{
				node temp_node;
				temp_node.g_cost = g_cost;
				std::pair<std::string, std::pair<int, int>> node_info = m_world->return_building_id(i);
				temp_node.building_id = node_info.first;
				temp_node.location = node_info.second;
				get_f_cost(temp_node, end_node);

				if (temp_node.f_cost == 0)
				{
					path_found = true;
				}

				bool skip = true;
				for (int e = 0; e < open_list.size(); e++)
				{
					if (open_list[e].current_node_num == i)
					{
						skip = open_list[e] < temp_node;
						break;
					}
				}

				if (!skip)
				{
					bool add_to_open = false;
					double closed_f_cost = closed_list(open_list.begin()->current_node_num, i);
					if (closed_f_cost != 0.0)
					{
						add_to_open = closed_f_cost < temp_node.f_cost;
					}

					if (add_to_open)
					{
						temp_node.current_node_num = i;
						temp_node.previous_node_num = open_list.begin()->current_node_num;
						open_list.push_back(temp_node);
					}
				}
			}
		}
		closed_list(open_list.begin()->current_node_num, open_list.begin()->current_node_num) = open_list.begin()->f_cost;
		open_list.erase(open_list.begin());
	}


}

std::string Movement::find_pre_made_path(std::pair<int, int> start, std::pair<int, int> end)
{
	return m_paths[std::make_pair(start, end)];
}

bool Movement::check_valid_path(std::string& path_id)
{
	std::shared_ptr<path> trial_path = m_valid_paths[path_id];
	
	//assume start node and end node have already been checked to see if they're open
	for (int i = 0; i < trial_path->nodes.size() - 1 ; i++)
	{
		if ((*m_transport_matrix)(trial_path->nodes[i]->current_node_num, trial_path->nodes[i + 1]->current_node_num) != 0)
		{
			continue;
		}
		else
		{
			char* message;
			std::string s_message = "PATH " + path_id + " NO LONGER VALID";
			message = &s_message[0];
			Log::info(message);

			m_valid_paths.erase(path_id);
			m_paths.erase(std::make_pair(trial_path->nodes[0]->location, trial_path->nodes[-1]->location));

			return false;
		}

	}
	return true;
}

Movement::Movement(unsigned int grid, std::shared_ptr<Enviroment> world)
	:m_grid_size(grid), m_world(world)
{
}

void Movement::get_f_cost(node& _node, node& _end_node)
{
	_node.h_cost = get_distance(_node.location, _end_node.location);
	_node.f_cost = _node.g_cost + _node.h_cost;
}

double Movement::partition(std::vector<node>& arr, int low, int high)
{
	int random_pivot = random::Random_number(low, high);
	node temp = arr[high];
	arr[high] = arr[random_pivot];
	arr[random_pivot] = temp;

	double pivot = arr[high].f_cost;

	int l = low - 1;

	for (int h = low; h <= high - 1; h++)
	{
		if (arr[h].f_cost <= pivot)
		{
			l++;
			node temp = arr[h];
			arr[h] = arr[l];
			arr[l] = temp;
		}
	}
	temp = arr[high];
	arr[high] = arr[l + 1];
	arr[l + 1] = temp;
	return(l + 1);
}

void Movement::quicksort(std::vector<node>& arr, int low, int high)
{
	if (low < high)
	{
		int pivot = partition(arr, low, high);
		quicksort(arr, low, pivot - 1);
		quicksort(arr, pivot + 1, high);
	}
}

std::pair<unsigned int, unsigned int> Movement::agent_random_walk(std::shared_ptr<Agent>& target_agent)
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

std::pair<unsigned int, unsigned int> Movement::agent_weighted_walk(std::shared_ptr<Agent>& target_agent)
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

Matrix<double> Movement::generate_move_matrix(std::shared_ptr<Agent> target_agent, bool random)
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
