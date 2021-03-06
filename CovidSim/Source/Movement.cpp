#include "Movement.h"

int Movement::update_agent(std::shared_ptr<Agent> target_agent, bool a_star, bool new_path, bool start, bool end)
{
	int result = 0;
	Agent::task_state mode = target_agent->get_task_state();
	std::pair<unsigned int, unsigned int> new_location;

	switch (mode)
	{
	case Agent::task_state::IDLE:
		new_location = agent_random_walk(target_agent);
		break;
	case Agent::task_state::TRANSIT:
		if (a_star == false)
		{
			new_location = agent_weighted_walk(target_agent);
			std::pair<unsigned int, unsigned int> location;
			if (start == true)
			{
				location = m_valid_paths[m_active_paths[target_agent->agent_id]]->start_node->location;
			}
			else if (end == true)
			{
				location = target_agent->get_task_location();
			}

			if (new_location == location)
			{
				result = 3;
			}
			
		}
		else if(new_path == true)
		{
			result = movement_planning(target_agent);
			if (result == 1 || result == 2)
			{
				target_agent->modify_permission();
				target_agent->set_target_location(m_valid_paths[m_active_paths[target_agent->agent_id]]->start_node->location);
				target_agent->modify_permission();
			}
		}
		else
		{
			bool end = false;
			//new_location = a_star_iterate(target_agent, end);
			if (end == true)
			{
				result = 4;
				target_agent->modify_permission();
				target_agent->set_target_location(target_agent->get_task_location());
				target_agent->modify_permission();
			}
		}
		break;
	case Agent::task_state::ACTIVE:
		break;
	default:
		break;
	}

	if ((mode == Agent::task_state::IDLE || mode == Agent::task_state::TRANSIT) && (a_star == false || new_path == false))
	{
		m_world->edit_agent_location(new_location, target_agent);
	}

	return result;
}

int Movement::update_agent(std::string target_agent, bool a_star, bool new_path, bool start, bool end)
{
	return 0;
}

int Movement::a_star(node& start_node, node& end_node, std::shared_ptr<path> valid_path)
{
	std::vector<node> closed_list;
	std::vector<node> open_list;

	start_node.g_cost = 0;
	get_f_cost(start_node, end_node);
	open_list.push_back(start_node);

	bool path_found = false;
	bool no_path_found = false;
	bool max_path_length = false;

	int run = 0;

	while (!path_found)
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
					for (int e = 0; e < closed_list.size(); e++)
					{
						if (closed_list[e].current_node_num == i)
						{
							add_to_open = closed_list[e].f_cost < temp_node.f_cost;
						}
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
		closed_list.push_back(open_list[0]);
		open_list.erase(open_list.begin());

		if (closed_list[-1].current_node_num == end_node.current_node_num)
		{
			path_found = true;
		}

		no_path_found = false;
		if (open_list.size() == 0)
		{
			Log::warning("NO VALID PATH FOUND");
			path_found = true;
			no_path_found = true;
		}

		if (run == CONSTANTS::MAX_PATH_LENGTH)
		{
			Log::info("REACHED MAX PATH LENGTH");
			path_found = true;
			max_path_length = true;
		}
	}

	if (path_found && !no_path_found)
	{
		path_found = false;
		no_path_found = false;
		valid_path->start_node = std::make_shared<node>(start_node);
		valid_path->end_node = std::make_shared<node>(end_node);

		int node_num = closed_list[-1].current_node_num;
		int index = 0;

		while (!(path_found && no_path_found))
		{
			for (int i = closed_list.size() - 1; i >= 0; i--)
			{
				if (closed_list[i].current_node_num == node_num)
				{
					valid_path->nodes.insert(valid_path->nodes.begin() + index, std::make_unique<node>(closed_list[i]));
					node_num = closed_list[i].previous_node_num;

					if (closed_list[i].current_node_num == start_node.current_node_num)
					{
						path_found = true;
					}

					break;
				}
				else if(i == 0)
				{
					no_path_found = true;
				}
			}
		}

		if (no_path_found == true)
		{
			Log::info("NO PATH FOUND");
			return 0;
		}

		
	}

	if (no_path_found)
	{
		Log::info("NO PATH FOUND");
		return 0;
	}
	else if (path_found && !max_path_length)
	{
		return 1;
	}
	else if (path_found && max_path_length)
	{
		return 2;
	}
	
	return 0;
}

int Movement::movement_planning(std::shared_ptr<Agent>& target_agent)
{
	int start = get_nearest_node(target_agent->get_location());
	int end = get_nearest_node(target_agent->get_target_location());
	std::string path_id = find_pre_made_path(start, end);
	bool valid_path = false;
	if (path_id != "")
	{
		valid_path = check_valid_path(path_id);
	}
	else
	{
		valid_path = false;
	}
	
	if (valid_path == false)
	{
		node start_node;
		start_node.current_node_num = start;
		start_node.building_id = m_world->return_building_id(start).first;

		node end_node;
		end_node.current_node_num = end;
		end_node.building_id = m_world->return_building_id(end).first;

		std::shared_ptr<path> a_star_path(new path);

		int result = 0;
		result = a_star(start_node, end_node, a_star_path); 

		if (result == 0)
		{
			return 0;
		}
		else if (result == 1)
		{
			const void* address = (const void*)a_star_path.get();
			std::stringstream ss;
			ss << address;
			std::string id = ss.str();

			m_paths[std::make_pair(start, end)] = id;
			m_valid_paths[id] = a_star_path;
			m_active_paths[target_agent->agent_id] = id;

			return 1;
		}
		else if (result == 2)
		{
			const void* address = (const void*)a_star_path.get();
			std::stringstream ss;
			ss << address;
			std::string id = ss.str();

			m_paths[std::make_pair(start, a_star_path->end_node->current_node_num)] = id;
			m_valid_paths[id] = a_star_path;
			m_active_paths[target_agent->agent_id] = id;

			return 2;
		}
	}
	else
	{
		m_active_paths[target_agent->agent_id] = path_id;
		return 1;
	}
	
	return 0;

}

int Movement::get_nearest_node(std::pair<int, int> location)
{
	std::vector<std::pair<double, int>> distance_vec;
	for (int i = 0; i < m_transport_matrix->get_row_size(); i++)
	{
		distance_vec.push_back(std::make_pair(get_distance(location, m_world->return_building_id(i).second), i));
	}
	quicksort(distance_vec, 0, distance_vec.size());
	return distance_vec[0].second;
}

std::string Movement::find_pre_made_path(int start, int end)
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
			m_paths.erase(std::make_pair(trial_path->nodes[0]->current_node_num, trial_path->nodes[-1]->current_node_num));

			return false;
		}

	}
	return true;
}

std::pair<unsigned int, unsigned int> Movement::a_star_iterate(std::shared_ptr<Agent>& target_agent, bool& finished, bool start)
{
	if (start == false)
	{

	}
	return { 0,0 };
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

double Movement::partition(std::vector<std::pair<double, int>>& arr, int low, int high)
{
	int random_pivot = random::Random_number(low, high);
	std::pair<double, int> temp = arr[high];
	arr[high] = arr[random_pivot];
	arr[random_pivot] = temp;

	double pivot = arr[high].first;

	int l = low - 1;

	for (int h = low; h <= high - 1; h++)
	{
		if (arr[h].first <= pivot)
		{
			l++;
			std::pair<double, int> temp = arr[h];
			arr[h] = arr[l];
			arr[l] = temp;
		}
	}
	temp = arr[high];
	arr[high] = arr[l + 1];
	arr[l + 1] = temp;
	return(l + 1);
}

void Movement::quicksort(std::vector<std::pair<double, int>>& arr, int low, int high)
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
