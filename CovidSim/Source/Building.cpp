#include "Building.h"

std::pair<bool, int> base_building::find(const std::shared_ptr<Agent>& agent)
{
    int position = 0;
    for (auto it = m_in_building.begin(); it < m_in_building.end(); it++)
    {
        if (*it == agent)
        {
            return std::make_pair(true, position);
        }
        position++;
    }
    return std::make_pair(false, 0);
}

void base_building::modify_closed(bool state)
{
    m_closed_lock.lock();
    m_closed = state;
    m_closed_lock.unlock();
}

base_building::base_building()
{
    m_x = 0;
    m_y = 0;
    m_closed = true;
    m_active_tasks = 0;
    m_in_building = {};
    m_capacity = 0;
}

void base_building::modify_active_tasks(int tasks)
{
    m_active_task_lock.lock();
    m_active_tasks = tasks;
    m_active_task_lock.unlock();
}

base_building::~base_building()
{
    m_in_building.clear();
}

std::pair<int, int> base_building::get_location()
{
    return std::make_pair(m_x, m_y);
}

std::vector<std::shared_ptr<Agent>> base_building::get_occupants()
{
    return m_in_building;
}

std::shared_ptr<Agent> base_building::get_occupant(const std::shared_ptr<Agent>& agent)
{
    std::pair<bool, int> occupant = find(agent);
    if (occupant.first == false)
    {
        Log::warning("AGENT NOT IN BUILDING");
        return nullptr;
    }
    return m_in_building[occupant.second];
}

void base_building::add_to_building(std::shared_ptr<Agent> agent)
{
    std::pair<bool, int> occupant = find(agent);
    if (occupant.first == true)
    {
        Log::warning("AGENT ALREADY IN BUILDING");
        return;
    }
    m_occupant_lock.lock();
    m_in_building.push_back(std::move(agent));
    m_occupant_lock.unlock();
}

void base_building::remove_from_building(std::shared_ptr<Agent>& agent)
{
   std::pair<bool, int> occupant = find(agent);
   if (occupant.first == false)
   {
       Log::warning("AGENT NOT IN BUILDING");
       return;
   }
   m_occupant_lock.lock();
   m_in_building.erase(m_in_building.begin() + occupant.second);
   m_occupant_lock.unlock();
}


