#pragma once

#include "Event.h"

namespace Events
{

	class GUI_Overlay_Event : public Event
	{
	public:
		GUI_Overlay_Event()
		{
		}


		EVENT_CLASS_TYPE(GUI_Overlay)
		EVENT_CLASS_CATEGORY(Event_Catagory_Menu)
	};

	class GUI_Building_Select_Event : public Event
	{
	public:
		GUI_Building_Select_Event()
		{
		}


		EVENT_CLASS_TYPE(GUI_Building_Select)
		EVENT_CLASS_CATEGORY(Event_Catagory_Menu)
	};
}