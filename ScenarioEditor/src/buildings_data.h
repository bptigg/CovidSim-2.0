#pragma once

#include <tuple>
#include <vector>

enum class SCHOOLS
{
	NURSARY = 0, 
	PRIMARY, 
	SECONDARY,  
	UNI, 
	NONE
};

struct education_buildings
{
	SCHOOLS type;
	std::pair<int, int> location;
	int staff_amount;
	int capacity;
};

enum class PUBLIC_BUILDINGS
{
	POW = 0, 
	PARK, 
	CAFE, 
	RESTAURANT, 
	CINEMA, 
	THEATRE, 
	GENERIC_SHOP, 
	SUPERMARKET, 
	SHOPPING_CENTRE, 
	PUB, 
	NIGHTCLUB, 
	ARENA, 
	GENERIC_WORK,
	HOSPITAL,
	DEFUALT
};

struct public_buildings
{
	int zone_code;
	std::pair<int, int> location;
	int staff_amount;
	int capacity;
};

enum class TRANSPORT_BUILDINGS
{
	BUS = 0,
	LIGHT_RAIL,
	RAPID_TRANSIT,
	TRAINS,
	NONE
};

struct transport_buildings
{
	std::vector<TRANSPORT_BUILDINGS> types;
	std::pair<int, int> location;
	int staff_amount;
	int capacity;

	bool interchange;
};