//#include "log.h"
#include "Matrix.h"
#include <random>

#include "Enviroment.h"
#include "Movement.h"
#include "Infection.h"
#include <Windows.h>

int main() 
{
	//Log::info("Hello world");
	//Log::error("NO ERROR","CovidSim.cpp",6);

	//Matrix<int> test_matrix(3,3,9);
	//Matrix<int> test_2 = test_matrix + 4;
	//Matrix<int> test_3 = test_matrix + test_2;

	//test_matrix = test_matrix + 4;

	//std::cout << test_3 << std::endl;

	//auto row = test_matrix(2);


	std::vector<Agent*> agents = {};
	Agent* agent = new Agent[100];
	for (int i = 0; i < 100; i++)
	{
		agents.push_back(&agent[i]);
		int x = random::Random_number(0, 20);
		int y = random::Random_number(0, 20, std::vector<int>{x});
		agent[i].set_location(std::make_pair(x,y));
		Sleep(1);
	}
	
	Enviroment world(20);
	world.initilise_agent_location(agents);
	
	Movement move(20, &world);
	Infection infect(&world);
	infect.set_radius(2);

	agents[0]->set_infection_state(Agent::infection_state::INFECTED);
	infect.set_infection_vector(std::vector<Agent*>{agents[0]});

	unsigned int count = 0;
	unsigned int max_count = 7 * CONSTANTS::DAY_LENGTH;

	
#if DEBUG
	Log::info("DEBUG BUILD");
#elif !DEBUG
	Log::info("RELEASE BUILD");
#endif

	Log::info("Model starts");

	while (count <= max_count)
	{
		for (int i = 0; i < agents.size(); i++)
		{
			move.update_agent(agents[i]);
		}
		infect.run_infection(Infection::infection_type::OUTDOORS);
		count++;
	}
	Log::info("DONE");
	
	agents.clear();
	delete[] agent;
	
}
