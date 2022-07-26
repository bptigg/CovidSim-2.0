#pragma once

#include "buildings_data.h"
#include "Log.h"

struct scenario_data
{
    std::string name;
    unsigned int max_counts;

    std::string population_pyramid;
    std::string population_race_dist;

    std::string adult_medical_data;
    std::string child_medical_data;
};

struct buildings
{
    std::vector<education_buildings> schools;
    std::vector<public_buildings> pub_buildings;
    std::vector<transport_buildings> transport;
};

class file_data
{
public:
    bool file_exists;
    std::fstream scenario_file;
    std::filesystem::path m_filepath;
    
    scenario_data scenario;
    buildings world_buildings;
private:
    static file_data* s_instance;
public:
    static file_data& get_file() { return *s_instance; }

    file_data();
    ~file_data();
};

