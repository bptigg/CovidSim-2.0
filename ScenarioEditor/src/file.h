#pragma once

#include "buildings_data.h"
#include "Log.h"

struct scenario_data
{
    std::string name;
    unsigned int population;
    unsigned int max_counts;

    std::string population_pyramid;
    std::string population_race_dist;

    std::string adult_medical_data;
    std::string child_medical_data;
};

struct world_data
{
    unsigned int num_of_tiles;
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
    world_data world_buildings;
private:
    static file_data* s_instance;
public:
    static file_data& get_file() { return *s_instance; }

    file_data();
    ~file_data();
};

