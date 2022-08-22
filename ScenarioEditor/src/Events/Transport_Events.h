#pragma once

#include "Event.h"

namespace Events
{

	class Transport_Overlay_Event : public Event
	{
	public:
		Transport_Overlay_Event()
		{
		}


		EVENT_CLASS_TYPE(Transport_Overlay_Select)
		EVENT_CLASS_CATEGORY(Event_Catagory_Transport)
	};

};