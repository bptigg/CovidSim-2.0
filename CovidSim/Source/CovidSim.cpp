//#include "log.h"
#include "Matrix.h"
#include <random>

#include "Enviroment.h"
#include "Movement.h"
#include "Infection.h"
#include "Output.h"
#include <Windows.h>

//#include "Building.h"
#include "Building types/public_building.h"
//This is testing enviroment

class Control_model_threads
{
private:
	bool m_kill_model = false;

	std::mutex m_model_lock;
public: 
	const bool& kill_model = m_kill_model;

	void modify_kill(bool state)
	{
		m_model_lock.lock();
		m_kill_model = state;
		m_model_lock.unlock();
	}
};

void esc_check(Control_model_threads* cmt)
{
	while (GetAsyncKeyState(VK_ESCAPE) == 0 || cmt->kill_model == true)
	{

	}

	Log::warning("MODEL ENDED BY USER");
	cmt->modify_kill(true);
};

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


	std::vector<std::shared_ptr<Agent>> agents = {};
	for (int i = 0; i < 200; i++)
	{
		std::shared_ptr<Agent> agent(new Agent);
		agents.push_back(std::move(agent));
		int x = random::Random_number(0, 40);
		int y = random::Random_number(0, 40, std::vector<int>{x});
		agents[i]->set_location(std::make_pair(x,y));
		Sleep(1);
	}

	//agent[0].set_target_location(std::make_pair(20,30));
	
	std::shared_ptr<Enviroment> world(new Enviroment(40, 200, CONSTANTS::DAY_OF_THE_WEEK::MONDAY));
	world->initilise_agent_location(agents);
	
	std::shared_ptr<Movement> move(new Movement(40, world));
	std::shared_ptr<Infection> infect(new Infection(world,1));
	infect->set_radius(2);

	output data_out(output::DATA::ON, output::DATA::OFF, output::DATA::ON, output::DATA::ON, infect);

	agents[0]->set_infection_state(Agent::infection_state::INFECTED);
	agents[0]->set_recovery_time(random::Normal_distribution(CONSTANTS::AVG_INFECTION_TIME, CONSTANTS::INFECTION_SD));
	infect->set_infection_vector(std::vector<std::shared_ptr<Agent>>{agents[0]});

	unsigned int count = 0;
	unsigned int max_count = 21 * CONSTANTS::DAY_LENGTH;

	//std::shared_ptr<Agent> test(agents[0]);

	//public_building* pb_test = new public_building;
	//std::vector<std::pair<CONSTANTS::DAY_OF_THE_WEEK, std::pair<int, int>>> hours = { std::make_pair(CONSTANTS::DAY_OF_THE_WEEK::MONDAY, std::make_pair(480,1080)) };
	//pb_test->update_opening_hours(hours);
	//pb_test->add_to_building(test);
	//pb_test->add_to_building(test);
	//pb_test->get_occupant(test);
	//pb_test->remove_from_building(test);
	//pb_test->get_occupant(test);
	//pb_test->update_building(500, CONSTANTS::DAY_OF_THE_WEEK::MONDAY);
	//pb_test->update_building(1200, CONSTANTS::DAY_OF_THE_WEEK::MONDAY);
	// pb_test;

	
#if _DEBUG 
	Log::info("DEBUG BUILD");
#elif !_DEBUG
	Log::info("RELEASE BUILD");
#endif

	Log::info("MODEL STARTS");

	Output_Lock_Guard olg;
	Control_model_threads cmt; 
	std::thread data_output(&output::output_data, &data_out, &olg);
	std::thread check_esc(&esc_check, &cmt);


	while (count <= max_count and cmt.kill_model == false)
	{
		for (int i = 0; i < agents.size(); i++)
		{
			move->update_agent(agents[i]);
		}
		while (olg.data_collected == false);
		infect->run_infection(Infection::infection_type::OUTDOORS);
		olg.modify_dc(false);
		count++;
		olg.modify_write(true);
	}
	olg.modify_kill(true);
	cmt.modify_kill(true);
	data_output.join();
	check_esc.join();
	Log::info("DONE");
	
	agents.clear();
	//delete[] agent;
	
}

