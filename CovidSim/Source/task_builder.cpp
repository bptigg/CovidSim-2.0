#include "task_builder.h"

TASK_CONSTANTS::public_building_type task_builder::generate_random_task(std::vector<int> age_range, int time, CONSTANTS::DAY_OF_THE_WEEK day, double weather_score)
{
    double n_weather_score = normalize_weather_modifier(weather_score);
    int avg_age = mean_age(age_range);

    if (n_weather_score >= 0.5)
    {
        n_weather_score = n_weather_score * 2;
    }

    std::map<TASK_CONSTANTS::public_building_type, double> weight_map= {};

    TASK_CONSTANTS::public_building_type type;
    for (int i = 0; i < (int)TASK_CONSTANTS::public_building_type::DEFUALT_TYPE; i++)
    {
        double total_modifer;
        type = (TASK_CONSTANTS::public_building_type)i;
        double age_mod = 0;
        double time_mod = 0;
        double day_mod = 0;
        double weather_mod = 1;
        switch (type)
        {
        case TASK_CONSTANTS::public_building_type::POW:
            age_mod = normalize_age_modifier((double)avg_age, TASK_CONSTANTS::POW_ARANGE);
            time_mod = normalize_time_modifier((double)time, TASK_CONSTANTS::POW_RANGE);
            day_mod = normalize_day_modifier(day, TASK_CONSTANTS::POW_DRANGE);
            break;
        case TASK_CONSTANTS::public_building_type::PARK:
            age_mod = normalize_age_modifier((double)avg_age, TASK_CONSTANTS::PARK_ARANGE);
            time_mod = normalize_time_modifier((double)time, TASK_CONSTANTS::PARK_RANGE);
            day_mod = normalize_day_modifier(day, TASK_CONSTANTS::PARK_DRANGE);
            break;
        case TASK_CONSTANTS::public_building_type::CAFE:
            age_mod = normalize_age_modifier((double)avg_age, TASK_CONSTANTS::CAFE_ARANGE);
            time_mod = normalize_time_modifier((double)time, TASK_CONSTANTS::CAFE_RANGE);
            day_mod = normalize_day_modifier(day, TASK_CONSTANTS::CAFE_DRANGE);
            break;
        case TASK_CONSTANTS::public_building_type::RESTAURANT:
            age_mod = normalize_age_modifier((double)avg_age, TASK_CONSTANTS::RESTAURANT_ARANGE);
            time_mod = normalize_time_modifier((double)time, TASK_CONSTANTS::RESTAURANT_RANGE);
            day_mod = normalize_day_modifier(day, TASK_CONSTANTS::RESTAURANT_DRANGE);
            break;
        case TASK_CONSTANTS::public_building_type::CINEMA:
            age_mod = normalize_age_modifier((double)avg_age, TASK_CONSTANTS::CINEMA_ARANGE);
            time_mod = normalize_time_modifier((double)time, TASK_CONSTANTS::CINEMA_RANGE);
            day_mod = normalize_day_modifier(day, TASK_CONSTANTS::CINEMA_DRANGE);
            break;
        case TASK_CONSTANTS::public_building_type::THEATRE:
            age_mod = normalize_age_modifier((double)avg_age, TASK_CONSTANTS::THEATRE_ARANGE);
            time_mod = normalize_time_modifier((double)time, TASK_CONSTANTS::THEATRE_RANGE);
            day_mod = normalize_day_modifier(day, TASK_CONSTANTS::THEATRE_DRANGE);
            break;
        case TASK_CONSTANTS::public_building_type::GENERIC_SHOP:
            age_mod = normalize_age_modifier((double)avg_age, TASK_CONSTANTS::GENERAL_SHOP_ARANGE);
            time_mod = normalize_time_modifier((double)time, TASK_CONSTANTS::GENERAL_SHOP_RANGE);
            day_mod = normalize_day_modifier(day, TASK_CONSTANTS::GENERAL_SHOP_DRANGE);
            break;
        case TASK_CONSTANTS::public_building_type::SUPERMARKET:
            age_mod = normalize_age_modifier((double)avg_age, TASK_CONSTANTS::SUPERMARKET_ARANGE);
            time_mod = normalize_time_modifier((double)time, TASK_CONSTANTS::SUPERMARKET_RANGE);
            day_mod = normalize_day_modifier(day, TASK_CONSTANTS::SUPERMARKET_DRANGE);
            break;
        case TASK_CONSTANTS::public_building_type::SHOPPING_CENTRE:
            age_mod = normalize_age_modifier((double)avg_age, TASK_CONSTANTS::SHOPPING_CENTRE_ARANGE);
            time_mod = normalize_time_modifier((double)time, TASK_CONSTANTS::SHOPPING_CENTRE_RANGE);
            day_mod = normalize_day_modifier(day, TASK_CONSTANTS::SHOPPING_CENTRE_DRANGE);
            break;
        case TASK_CONSTANTS::public_building_type::PUB:
            age_mod = normalize_age_modifier((double)avg_age, TASK_CONSTANTS::PUB_ARANGE, true);
            time_mod = normalize_time_modifier((double)time, TASK_CONSTANTS::PUB_RANGE);
            day_mod = normalize_day_modifier(day, TASK_CONSTANTS::PUB_DRANGE);
            break;
        case TASK_CONSTANTS::public_building_type::NIGHTCLUB:
            age_mod = normalize_age_modifier((double)avg_age, TASK_CONSTANTS::NIGHTCLUB_ARANGE, true);
            time_mod = normalize_time_modifier((double)time, TASK_CONSTANTS::NIGHTCLUB_RANGE);
            day_mod = normalize_day_modifier(day, TASK_CONSTANTS::NIGHTCLUB_DRANGE);
            break;
        case TASK_CONSTANTS::public_building_type::ARENA:
            age_mod = normalize_age_modifier((double)avg_age, TASK_CONSTANTS::ARENA_ARANGE, true);
            time_mod = normalize_time_modifier((double)time, TASK_CONSTANTS::ARENA_RANGE);
            day_mod = normalize_day_modifier(day, TASK_CONSTANTS::ARENA_DRANGE);
            break;
        case TASK_CONSTANTS::public_building_type::HOME:
            age_mod = normalize_age_modifier((double)avg_age, TASK_CONSTANTS::HOME_ARANGE);
            time_mod = normalize_time_modifier((double)time, TASK_CONSTANTS::HOME_RANGE);
            day_mod = normalize_day_modifier(day, TASK_CONSTANTS::HOME_DRANGE);
            break;
        case TASK_CONSTANTS::public_building_type::DEFUALT_TYPE:
            break;
        default:
            break;
        }

        if (type == TASK_CONSTANTS::public_building_type::PARK)
        {
            weather_mod = weather_mod * n_weather_score;
        }
        else if (type == TASK_CONSTANTS::public_building_type::HOME)
        {
            weather_mod = weather_mod / (2 * n_weather_score);
        }
        else
        {
            weather_mod = weather_mod / n_weather_score;
        }
        
        total_modifer = 1 + age_mod + time_mod + day_mod + weather_mod;
        if (age_mod == 0)
        {
            continue;
        }
        
        weight_map[(TASK_CONSTANTS::public_building_type)i] = {total_modifer};
    }

    std::vector<double> weights;
    double sum = 0;
    int i = 0;
    if (weight_map.size() != 0)
    {
        for (auto it = weight_map.begin(); it != weight_map.end(); it++)
        {
            weights.push_back(it->second * (1.0 / weight_map.size()));
            sum = sum + weights[i];
            i++;
        }
        for (int i = 0; i < weights.size(); i++)
        {
            weights[i] = weights[i] / sum;
        }

        int selected_task = random::Discrete_distribution(weights, 1, true)[0];
        return (TASK_CONSTANTS::public_building_type)selected_task;
    }

    return TASK_CONSTANTS::public_building_type::DEFUALT_TYPE; //not pernament
}

std::string task_builder::get_task_building(std::pair<int,int> location, TASK_CONSTANTS::public_building_type building, double weather_mod, std::vector<std::shared_ptr<Agent>> target_group)
{
    std::string buildingid;
    if (building != TASK_CONSTANTS::public_building_type::HOME)
    {
        double n_weather_mod = normalize_weather_modifier(weather_mod);
        int i = 0;
        std::vector<std::pair<double, int>> dist_vec;
        std::map<std::string, double> distance;
        std::map<std::string, std::shared_ptr<public_building>> p_building = m_enviroment->return_public_list(building);
        for (auto it = p_building.begin(); it != p_building.end(); it++)
        {
            double dist = get_distance(location, it->second->get_location());
            distance[it->first] = dist;
            dist_vec.push_back(std::make_pair(dist, i));
            i++;
        }

        int result = 0;
        if (dist_vec.size() > 1)
        {
            quicksort(dist_vec, 0, dist_vec.size() - 1);
            std::pair<double, double> dist_range = std::make_pair(dist_vec[0].first / n_weather_mod, (dist_vec[-1].first + 1) / n_weather_mod);
            double sum = 0;
            for (int e = 0; e < dist_vec.size(); e++)
            {
                dist_vec[e].first = dist_vec[e].first / n_weather_mod;
                dist_vec[e].first = normalize_distance(dist_vec[e].first, dist_range);
                sum = sum + dist_vec[e].first;
            }
            double mod = 1 / sum;
            std::vector<double> weights = {};
            for (int e = 0; e < dist_vec.size(); e++)
            {
                weights.push_back(dist_vec[e].first * mod);
            }
            result = random::Discrete_distribution(weights, 1, true)[0];
            result = dist_vec[result].second;
        }
        int a = 0;
        for (auto it = distance.begin(); it != distance.end(); it++)
        {
            if (a = result)
            {
                buildingid = it->first;
                return buildingid;
            }
            a++;
        }
        
    }
    else if (building == TASK_CONSTANTS::public_building_type::HOME)
    {
        std::map<std::string, double> distance;
        std::vector<std::pair<double, int>> dist_vec;
        for (int i = 0; i < target_group.size(); i++)
        {
            std::string house_id = target_group[i]->house_id;
            double dist = get_distance(location, m_enviroment->return_house(house_id)->get_location());
            distance[house_id] = dist;
            dist_vec.push_back(std::make_pair(dist, i));
        }

        int result = 0;
        if (dist_vec.size() > 1)
        {
            quicksort(dist_vec,0,dist_vec.size()-1);
            double sum = 0;
            for (int i = 0; i < dist_vec.size(); i++)
            {
                dist_vec[i].first = normalize_distance(dist_vec[i].first, std::make_pair(dist_vec[0].first, dist_vec[-1].first));
                sum = sum + dist_vec[i].first;
            }
            double mod = 1 / sum;
            std::vector<double> weights;
            for (int i = 0; i < dist_vec.size(); i++)
            {
                weights.push_back(dist_vec[i].first * mod);
            }
            result = random::Discrete_distribution(weights, 1, true)[0];
            result = dist_vec[result].second;

        }
        int a = 0;
        for (auto it = distance.begin(); it != distance.end(); it++)
        {
            if (a == result)
            {
                buildingid = it->first;
                return buildingid;
            }
            a++;
        }
    }

    buildingid = "";
    return buildingid;
}

double task_builder::normalize_weather_modifier(double weather_score)
{
    return (weather_score - m_enviroment->min_weather_score) / (m_enviroment->max_weather_score - m_enviroment->min_weather_score);
}

double task_builder::normalize_age_modifier(double mean_age, std::pair<double, double> age_bracket, bool age_limit)
{
    if (mean_age >= age_bracket.first && mean_age <= age_bracket.second)
    {
        return 1.0;
    }
    
    if (mean_age < age_bracket.first && age_limit == false)
    {
        return (mean_age - TASK_CONSTANTS::MIN_AGE) / (age_bracket.first - TASK_CONSTANTS::MIN_AGE);
    }
    else if (mean_age > age_bracket.second)
    {
        return (mean_age - age_bracket.second) / (TASK_CONSTANTS::MAX_AGE - age_bracket.second);
    }
    else
    {
        return 0;
    }
}

double task_builder::normalize_time_modifier(double time, std::pair<double, double> peak_time)
{
    if (time >= peak_time.first && time <= peak_time.second)
    {
        return 1.0;
    }
    if (time < peak_time.first)
    {
        return (time) / (peak_time.first);
    }
    else if (time > peak_time.second)
    {
        return (time - peak_time.second) / (CONSTANTS::DAY_LENGTH - peak_time.second);
    }
    else
    {
        return 0;
    }
}

double task_builder::normalize_day_modifier(CONSTANTS::DAY_OF_THE_WEEK day, std::pair<CONSTANTS::DAY_OF_THE_WEEK, CONSTANTS::DAY_OF_THE_WEEK> peak_day)
{
    double day2 = (double)day;
    double min_day = (double)peak_day.first;
    double max_day = (double)peak_day.second;

    if (day2 >= min_day && day2 <= max_day);
    {
        return 1.0;
    }
    if (day2 < min_day)
    {
        return (day2) / (min_day);
    }
    else if (day2 > max_day && max_day != 0)
    {
        return (day2 - max_day) / (6 - max_day);
    }
    else
    {
        return 0;
    }
}

double task_builder::normalize_distance(double distance, std::pair<double, double> disance_bracket)
{
    return std::abs(((distance - disance_bracket.first) / (disance_bracket.second - disance_bracket.first))-1);
}

int task_builder::mean_age(std::vector<int>& age_range)
{
    double sum = 0;
    for (int i = 0; i < age_range.size(); i++)
    {
        sum = sum + age_range[i];
    }

    return (int)std::floor(sum / age_range.size());
}

double task_builder::get_distance(std::pair<int, int> agent, std::pair<int, int> building)
{
    int x_diff = building.first - agent.first;
    int y_diff = building.second - agent.second;
    return std::sqrt(std::pow(x_diff, 2) + std::pow(y_diff, 2));
}

double task_builder::partition(std::vector<std::pair<double, int>>& arr, int low, int high)
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
    arr[high] = arr[l+1];
    arr[l+1] = temp;
    return(l + 1);
}

void task_builder::quicksort(std::vector<std::pair<double, int>>& arr, int low, int high)
{
    if (low < high)
    {
        int pivot = partition(arr, low, high);
        quicksort(arr, low, pivot - 1);
        quicksort(arr, pivot + 1, high);  
    }
}


task_builder::task_builder(std::shared_ptr<Enviroment> env)
{
    task_initlized = false;
    m_active_tasks = 0;

    m_max_tasks = (std::thread::hardware_concurrency() / 8) * TASK_CONSTANTS::BASE_MAX_TASKS;
    m_tasks_extension = (std::thread::hardware_concurrency() / 8) * TASK_CONSTANTS::MAX_TASKS_EXTENSION;
    friend_task_setup_key = { "","" };

    m_enviroment = std::move(env);
}

bool task_builder::create_task(std::vector<std::shared_ptr<Agent>>& target_agents, std::shared_ptr<task> m_task)
{
    double sum_x = 0;
    double sum_y = 0;

    int i = 0;
    std::vector<int> age_range;
    for (i = 0; i < target_agents.size(); i++)
    {
        sum_x = sum_x + target_agents[i]->get_location().first;
        sum_y = sum_y + target_agents[i]->get_location().second;
        age_range.push_back(target_agents[i]->age);
    }

    std::pair<int, int> mean_location = {};
    std::pair<int, int> task_location = {};

    mean_location = std::make_pair(std::floor(sum_x / i), std::floor(sum_y / i));

    TASK_CONSTANTS::public_building_type task_type = generate_random_task(age_range, m_enviroment->time_of_day, m_enviroment->day, m_enviroment->get_weather_score());
    std::string building_id = get_task_building(mean_location, task_type, m_enviroment->get_weather_score(), target_agents);

    if (task_type != TASK_CONSTANTS::public_building_type::HOME)
    {
        std::map<std::string, std::shared_ptr<public_building>> id_table = m_enviroment->return_public_list(task_type);

        task_location = id_table[building_id]->get_location();
    }
    else if (task_type == TASK_CONSTANTS::public_building_type::HOME)
    {
        std::shared_ptr<house> id_table = m_enviroment->return_house(building_id);

        task_location = id_table->get_location();
    }
    else
    {
        Log::warning("VALID TASK CREATION FAILED: NO BUILDING ID FOUND");
        return false;
    }

    int task_length = random::Random_number(50, 1000);

    if (m_task->initlize_task(task_location, building_id, target_agents, task_length) == false)
    {
        Log::warning("FAILED TO CREATE TASK");
        return false;
    }

    m_task_list[m_task->task_id] = m_task;

    for (int e = 0; e < target_agents.size(); e++)
    {
        target_agents[i]->set_task_id(m_task->task_id);
    }

    return true;
    
}

bool task_builder::friend_task_setup()
{
    std::pair<std::pair<std::string, std::string>, std::vector<std::string>> pairing;
    std::vector<std::shared_ptr<Agent>> target_agents;
    if (m_function_depth == 0)
    {
       pairing = m_friendship_director->get_viable_pairing(2);
       friend_task_setup_key = pairing.first;
    }
    else
    {
        pairing = m_friendship_director->get_viable_pairing(2, friend_task_setup_key);
    }

    if (pairing.first.first == "")
    {
        return false;
    }

    target_agents.push_back(m_friendship_director->agents[pairing.first.first]);
    target_agents.push_back(m_friendship_director->agents[pairing.first.second]);

    for (int i = 0; i < pairing.second.size(); i++)
    {
        target_agents.push_back(m_friendship_director->agents[pairing.second[i]]);
    }

    bool return_value = false;

    return_value = request_task(target_agents, true);
    
    m_function_depth = 0;
    friend_task_setup_key = { "","" };

    return return_value;
}

bool task_builder::request_task(std::vector<std::shared_ptr<Agent>>& target_agents, bool friend_task)
{
    int number_of_required_tasks = target_agents.size();
    if (m_active_tasks + number_of_required_tasks > m_max_tasks && friend_task != false)
    {
        return false;
    }
    else if (m_active_tasks + number_of_required_tasks > m_max_tasks && friend_task == false)
    {
        if (m_active_tasks + number_of_required_tasks - m_max_tasks < m_tasks_extension)
        {
            Log::info("TASK BUDGET EXCEEDED BY SUITABLE LIMIT");
        }
        else
        {
            m_function_depth++;
            if (m_function_depth <= TASK_CONSTANTS::TASK_RECURSION_LIMIT)
            {
                friend_task_setup();
            }
            else
            {
                Log::warning("TASK BUDGET EXCEEDED BY NON SUITABLE LIMIT");
                return false;
            }
        }
    }
    for (int i = 0; i < target_agents.size(); i++)
    {
        if (target_agents[i]->get_task_state() != Agent::task_state::IDLE)
        {
            return false;
        }
    }

    bool task_created = false;
    std::shared_ptr<task> agent_task(new task);
    task_created = create_task(target_agents, agent_task);
    if (task_created == true)
    {
        m_task_list[agent_task->task_id] = agent_task;
    }
    return task_created;
}

bool task_builder::request_task(std::vector<std::string>& target_agents, bool friend_task)
{
    std::vector<std::shared_ptr<Agent>> agent_list = {};
    for (int i = 0; i < target_agents.size(); i++)
    {
        agent_list.push_back(m_friendship_director->agents[target_agents[i]]);
    }

    bool return_value = request_task(agent_list, friend_task);
    return return_value;
}


