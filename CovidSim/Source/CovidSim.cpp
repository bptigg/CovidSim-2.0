//#include "log.h"
#include "Matrix.h"
#include <random>

#include "Enviroment.h"
#include "Movement.h"
#include "Infection.h"
#include "Output.h"
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
	Agent* agent = new Agent[200];
	for (int i = 0; i < 200; i++)
	{
		agents.push_back(&agent[i]);
		int x = random::Random_number(0, 40);
		int y = random::Random_number(0, 40, std::vector<int>{x});
		agent[i].set_location(std::make_pair(x,y));
		Sleep(1);
	}

	//agent[0].set_target_location(std::make_pair(20,30));
	
	Enviroment world(40,200);
	world.initilise_agent_location(agents);
	
	Movement move(40, &world);
	Infection infect(&world,1);
	infect.set_radius(2);

	output data_out(output::DATA::ON, output::DATA::OFF, output::DATA::ON, output::DATA::ON, &infect);

	agents[0]->set_infection_state(Agent::infection_state::INFECTED);
	agents[0]->set_recovery_time(random::Normal_distribution(CONSTANTS::AVG_INFECTION_TIME, CONSTANTS::INFECTION_SD));
	infect.set_infection_vector(std::vector<Agent*>{agents[0]});

	unsigned int count = 0;
	unsigned int max_count = 21 * CONSTANTS::DAY_LENGTH;
	bool write_to_file = false;
	bool kill_output = false;
	
#if _DEBUG
	Log::info("DEBUG BUILD");
#elif !_DEBUG
	Log::info("RELEASE BUILD");
#endif

	Log::info("Model starts");

	std::thread data_output(&output::output_data, data_out, &write_to_file, &kill_output);
	while (count <= max_count)
	{
		for (int i = 0; i < agents.size(); i++)
		{
			move.update_agent(agents[i]);
		}
		infect.run_infection(Infection::infection_type::OUTDOORS);
		count++;
		write_to_file = true;
	}
	kill_output = true;
	data_output.join();
	Log::info("DONE");
	
	agents.clear();
	delete[] agent;
	
}
