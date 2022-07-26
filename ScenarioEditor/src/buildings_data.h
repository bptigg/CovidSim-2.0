#pragma once

#include <tuple>

enum class SCHOOLS
{
	NURSARY = 0, 
	PRIMARY, 
	SECONDARY, 
	COLLEGE, 
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
	PUB, NIGHTCLUB, 
	ARENA, 
	GENERIC_WORK,
	HOSPITAL,
	DEFUALT
};

struct public_buildings
{
	PUBLIC_BUILDINGS type;
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
	AIRPORT,
	NONE
};

struct transport_buildings
{

	TRANSPORT_BUILDINGS type;
	std::pair<int, int> location;
	int staff_amount;
	int capacity;

	bool interchange;
};