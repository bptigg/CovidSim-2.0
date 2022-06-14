#include "task.h"

std::string task::get_id()
{
    const void* m_address = (const void*)this;
    std::stringstream ss;
    ss << m_address;
    return ss.str();
}

TASK_CONSTANTS::public_building_type task::generate_random_task(std::vector<int> age_range, int time, CONSTANTS::DAY_OF_THE_WEEK day, double weather_score)
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

std::string task::get_task_building(std::pair<int,int> location, TASK_CONSTANTS::public_building_type building, double weather_mod)
{
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
                normalize_distance(dist_vec[e].first, dist_range);
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
        std::string building_id;
        for (auto it = distance.begin(); it != distance.end(); it++)
        {
            if (a = result)
            {
                return it->first;
            }
            a++;
        }
        
    }
    //Home requires looking at the target group and picking a home out of that group

    return building_id;
}

double task::normalize_weather_modifier(double weather_score)
{
    return (weather_score - m_enviroment->min_weather_score) / (m_enviroment->max_weather_score - m_enviroment->min_weather_score);
}

double task::normalize_age_modifier(double mean_age, std::pair<double, double> age_bracket, bool age_limit)
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

double task::normalize_time_modifier(double time, std::pair<double, double> peak_time)
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

double task::normalize_day_modifier(CONSTANTS::DAY_OF_THE_WEEK day, std::pair<CONSTANTS::DAY_OF_THE_WEEK, CONSTANTS::DAY_OF_THE_WEEK> peak_day)
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

double task::normalize_distance(double distance, std::pair<double, double> disance_bracket)
{
    return std::abs(((distance - disance_bracket.first) / (disance_bracket.second - disance_bracket.first))-1);
}

int task::mean_age(std::vector<int>& age_range)
{
    double sum = 0;
    for (int i = 0; i < age_range.size(); i++)
    {
        sum = sum + age_range[i];
    }

    return (int)std::floor(sum / age_range.size());
}

double task::get_distance(std::pair<int, int> agent, std::pair<int, int> building)
{
    int x_diff = building.first - agent.first;
    int y_diff = building.second - agent.second;
    return std::sqrt(std::pow(x_diff, 2) + std::pow(y_diff, 2));
}

double task::partition(std::vector<std::pair<double, int>>& arr, int low, int high)
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

void task::quicksort(std::vector<std::pair<double, int>>& arr, int low, int high)
{
    if (low < high)
    {
        int pivot = partition(arr, low, high);
        quicksort(arr, low, pivot - 1);
        quicksort(arr, pivot + 1, high);  
    }
}


task::task(std::shared_ptr<Enviroment> env)
{
    m_target_x = 0;
    m_target_y = 0;
    m_building_id = "";
    m_target_agents = {};
    m_task_length = 0;
    m_current_run_time = 0; 

    m_task_id = get_id();

    task_initlized = false;

    m_enviroment = std::move(env);
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
