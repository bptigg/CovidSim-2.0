#include "task.h"

void task::create_id()
{
    const void* m_address = (const void*)this;
    std::stringstream ss;
    ss << m_address;
    m_task_id = ss.str();
}

task::task()
{
    m_target_location = { 0,0 };
    m_building_id = "";
    m_target_agents = {};
    m_task_length = 0;
    m_task_cduration = 0;
    
    task_initlized = false;

    create_id();
}

task::~task()
{
    m_target_agents.clear();
}

bool task::initlize_task(std::pair<int, int> location, std::string b_id, std::vector<std::shared_ptr<Agent>>& target_agents, int run_time)
{
    if (task_initlized == true)
    {
        std::string temp_message = "TASK" + m_task_id + "ALREADY INITLIZED";
        std::vector<char> chars(temp_message.begin(), temp_message.end());
        chars.push_back('\0');
        Log::info(&chars[0]);
        return false;
    }

    m_target_location = location;

    m_building_id = b_id;
    m_task_length = run_time;

    for (int i = 0; i < target_agents.size(); i++)
    {
        m_target_agents.push_back(std::move(target_agents[i]));
    }

    task_initlized = true;
    return true;
}

bool task::update_task(int run)
{
    m_task_cduration++;

    if (m_task_cduration >= m_task_length)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void task::update_task_length(int run_time)
{
    m_task_length = run_time;
}

void task::update_target_agents(std::vector<std::shared_ptr<Agent>>& target_agents)
{
    for (int i = 0; i < target_agents.size(); i++)
    {
        if (std::find(m_target_agents.begin(), m_target_agents.end(), target_agents[i]) == m_target_agents.end())
        {
            m_target_agents.push_back(std::move(target_agents[i]));
        }
        else
        {
            Log::warning("AGENT ALREADY A TARGET AGENT");
        }
    }
}

void task::update_target_agent(std::shared_ptr<Agent> target_agent)
{
    if (std::find(m_target_agents.begin(), m_target_agents.end(), target_agent) == m_target_agents.end())
    {
        m_target_agents.push_back(std::move(target_agent));
    }
    else
    {
        Log::warning("AGENT ALREADY A TARGET AGENT");
    }
}
