#include "task.h"

std::string task::get_id()
{
    const void* m_address = (const void*)this;
    std::stringstream ss;
    ss << m_address;
    return ss.str();
}

task::task()
{
    m_target_x = 0;
    m_target_y = 0;
    m_building_id = "";
    m_target_agents = {};
    m_task_length = 0;
    m_current_run_time = 0; 

    m_task_id = get_id();

    task_initlized = false;
}

task::~task()
{
    m_target_agents.clear();
}

bool task::update_task()
{
    m_current_run_time++;

    if (m_current_run_time >= m_task_length)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool task::initlize_task(int x, int y, std::string b_id, std::vector<std::shared_ptr<Agent>>& target_agents, int run_time)
{
    if (task_initlized == true)
    {
        std::string temp_message = "TASK" + m_task_id + "ALREADY INITLIZED";
        std::vector<char> chars(temp_message.begin(), temp_message.end());
        chars.push_back('\0');
        Log::info(&chars[0]);
        return false;
    }

    m_target_x = x;
    m_target_y = y;

    m_building_id = b_id;
    m_task_length = run_time;

    for (int i = 0; i < target_agents.size(); i++)
    {
        m_target_agents.push_back(std::move(target_agents[i]));
    }

    task_initlized = true;
    return true;
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

void task::update_target_agents(std::shared_ptr<Agent> target_agent)
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
