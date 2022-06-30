#pragma once

#include <memory>
#include <cstring>

#include <unordered_map>
#include <map>

#include "Agent.h"
#include "Matrix.h"
#include "Random.h"

#include "Enviroment.h"


struct node
{
	std::pair<int, int> location;
	std::string building_id;

	int previous_node_num;
	int current_node_num;

	double g_cost, h_cost, f_cost;

	bool operator<(const node& node_2)
	{
		return f_cost < node_2.f_cost;
	}
};

struct path
{
	std::shared_ptr<node> start_node;
	std::vector<std::unique_ptr<node>> nodes;
	std::shared_ptr<node> end_node;
};

class Movement
{
private:
	unsigned int m_grid_size; 
	std::shared_ptr<Enviroment> m_world;

	std::shared_ptr<Matrix<int>> m_transport_matrix;
	std::shared_ptr<Matrix<int>> m_world_matrix;

	std::unordered_map<std::string, std::shared_ptr<path>> m_valid_paths;
	std::map<std::pair<std::pair<int, int>, std::pair<int, int>>, std::string> m_paths;
	std::unordered_map<std::string, std::string> m_active_paths;

public:
	Movement(unsigned int grid, std::shared_ptr<Enviroment> world);
	void update_agent(std::shared_ptr<Agent> target_agent);

#pragma region TASK_MOVEMENT
	void a_star(node& start_node, node& end_node);

	std::string find_pre_made_path(std::pair<int, int> start, std::pair<int, int> end);
	bool check_valid_path(std::string& path_id);

	double get_distance(std::pair<int, int> first, std::pair<int, int> second) 
	{
		return std::sqrt(std::pow((second.first - first.first), 2) + std::pow((second.second - first.second), 2));
	}

	void get_f_cost(node& _node, node& _end_node);

	double partition(std::vector<node>& arr, int low, int high);
	void quicksort(std::vector<node>& arr, int low, int high);

#pragma endregion 
private:
	std::pair<unsigned int, unsigned int> agent_random_walk(std::shared_ptr<Agent>& target_agent);
	std::pair<unsigned int, unsigned int> agent_weighted_walk(std::shared_ptr<Agent>& target_agent);

	Matrix<double> generate_move_matrix(std::shared_ptr<Agent> target_agent, bool random = false);

	
};

