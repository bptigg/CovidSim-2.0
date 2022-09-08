#pragma once

#include <glm/glm.hpp>

namespace building_constants
{
	const glm::vec4 UNDEVLOPED_SPACE = { 0.3f, 0.3f, 0.3f,1.0f };
	const glm::vec4 WALKING_ZONE = { 0.5f, 0.5f, 0.5f, 1.0f };
	const glm::vec4 HOUSING_ZONE = { 86.0f / 255.0f, 125.0f / 255.0f, 70.0f / 255.0f, 1.0f };
	const glm::vec4 GENERIC_WORK_ZONE = { 64.0f/ 255.0F, 224.0f/ 255.0F, 208.0f/ 255.0f, 1.0f };
	const glm::vec4 GENERIC_SHOP_ZONE = { 51.0f / 255.0f, 51.0f / 255.0f, 1.0f, 1.0f };
	const glm::vec4 HOSPITAL = {};

	const glm::vec4 PLACE_OF_WORSHIP = {100.0f / 255.0f, 0.0f, 100.0f/ 200.0f, 1.0f};
	const glm::vec4 PARK = {2.0f / 255.0f, 48.0f/ 255.0f, 32.0f/ 255.0f, 1.0f};
	const glm::vec4 CAFE = {101.0f/ 255.0f, 67.0f / 255.0f, 33.0f / 255.0f, 1.0f};
	const glm::vec4 RESTAURANT = {167.0f / 255.0f, 199.0f / 255.0f, 231.0f / 255.0f, 1.0f};
	const glm::vec4 CINEMA = {236.0f / 255.0f, 9.0f / 255.0f, 9.0f / 255.0f, 1.0f};
	const glm::vec4 THEATRE = {177.0f / 255.0f, 39.0f / 255.0f, 74.0f / 255.0f, 1.0f};
	const glm::vec4 SUPERMARKET = {184.0f / 255.0f, 182.0f / 255.0f, 231.0f / 255.0f, 1.0f};
	const glm::vec4 SHOPPING_CENTRE = {231.0f / 255.0f, 212.0f / 255.0f, 182.0f / 255.0f, 1.0f};
	const glm::vec4 PUB = {250.0f / 255.0f, 186.0f / 255.0f, 89.0f / 255.0f, 1.0f};
	const glm::vec4 NIGHTCLUB = {57.0f / 255.0f, 39.0f / 255.0f, 102.0f / 255.0f, 1.0f};
	const glm::vec4 ARENA = {213.0f / 255.0f, 220.0f / 255.0f, 95.0f / 255.0f, 1.0f};

	const glm::vec4 BUS = {0.2f, 0.4f, 0.1f, 1.0f};
	const glm::vec4 LIGHT_RAIL = {0.45f, 0.23f, 0.9f, 1.0f};
	const glm::vec4 RAPID_TRANSIT = {0.9f, 0.2f, 0.5f, 1.0f};
	const glm::vec4 TRAINS = {0.4f, 0.2f, 0.1f, 0.8f};
}


unsigned int get_building_code(glm::vec4 colour)
{
	if (colour == building_constants::UNDEVLOPED_SPACE)
	{
		return 0;
	}
	else if (colour == building_constants::WALKING_ZONE)
	{
		return 1;
	}
	else if (colour == building_constants::HOUSING_ZONE)
	{
		return 2;
	}
	else if (colour == building_constants::GENERIC_WORK_ZONE)
	{
		return 3;
	}
	else if (colour == building_constants::GENERIC_SHOP_ZONE)
	{
		return 4;
	}
	else if (colour == building_constants::HOSPITAL)
	{
		return 5;
	}
	else if (colour == building_constants::PLACE_OF_WORSHIP)
	{
		return 6;
	}
	else if (colour == building_constants::PARK)
	{
		return 7;
	}
	else if (colour == building_constants::CAFE)
	{
		return 8;
	}
	else if (colour == building_constants::RESTAURANT)
	{
		return 9;
	}
	else if (colour == building_constants::CINEMA)
	{
		return 10;
	}
	else if (colour == building_constants::THEATRE)
	{
		return 11;
	}
	else if (colour == building_constants::SUPERMARKET)
	{
		return 12;
	}
	else if (colour == building_constants::SHOPPING_CENTRE)
	{
		return 13;
	}
	else if (colour == building_constants::PUB)
	{
		return 14;
	}
	else if (colour == building_constants::NIGHTCLUB)
	{
		return 15;
	}
	else if (colour == building_constants::ARENA)
	{
		return 16;
	}
	else if (colour == building_constants::BUS)
	{
		return 17;
	}
	else if (colour == building_constants::LIGHT_RAIL)
	{
		return 18;
	}
	else if (colour == building_constants::RAPID_TRANSIT)
	{
		return 19;
	}
	else if (colour == building_constants::TRAINS)
	{
		return 20;
	}
	else
	{
		return -1;
	}


}