#pragma once

#include <glm/glm.hpp>

namespace building_constants
{
	const glm::vec4 UNDEVLOPED_SPACE = { 0.3f, 0.3f, 0.3f,1.0f };
	const glm::vec4 WALKING_ZONE = { 0.5f, 0.5f, 0.5f, 1.0f };
	const glm::vec4 HOUSING_ZONE = { 86.0f / 255.0f, 125.0f / 255.0f, 70.0f / 255.0f, 1.0f };
	const glm::vec4 GENERIC_WORK_ZONE = { 51.0f / 255.0f, 51.0f / 255.0f, 1.0f, 1.0f };
	const glm::vec4 GENERIC_SHOP_ZONE = {};
	const glm::vec4 HOSPITAL = {};

	const glm::vec4 PLACE_OF_WORSHIP = {};
	const glm::vec4 PARK = {};
	const glm::vec4 CAFE = {};
	const glm::vec4 RESTAURANT = {};
	const glm::vec4 CINEMA = {};
	const glm::vec4 THEATRE = {};
	const glm::vec4 SUPERMARKET = {};
	const glm::vec4 SHOPPING_CENTRE = {};
	const glm::vec4 PUB = {};
	const glm::vec4 NIGHTCLUB = {};
	const glm::vec4 ARENA = {};
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
	else
	{
		return 0;
	}


}