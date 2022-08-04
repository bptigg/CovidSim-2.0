#pragma once

#include "Event.h"

#include "../Entity/scriptable_object.h"

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
		GUI_Building_Select_Event(scriptable_object* caller)
		{
			m_button = caller;
		}

		scriptable_object* get_caller() { return m_button; }


		EVENT_CLASS_TYPE(GUI_Building_Select)
		EVENT_CLASS_CATEGORY(Event_Catagory_Menu)

	private:
		scriptable_object* m_button;
	};

	class GUI_Public_Building_Event : public Event
	{
	public:
		GUI_Public_Building_Event(scriptable_object* caller, scriptable_object* menu)
		{
			m_button = caller;
			m_menu = menu;
		}

		scriptable_object* get_caller() { return m_button; }
		scriptable_object* get_menu() { return m_menu; }


		EVENT_CLASS_TYPE(GUI_Public_Select)
		EVENT_CLASS_CATEGORY(Event_Catagory_Menu)

	private:
		scriptable_object* m_button;
		scriptable_object* m_menu;
	};

	class GUI_Transport_Building_Event : public Event
	{
	public:
		GUI_Transport_Building_Event(scriptable_object* caller)
		{
			m_button = caller;
		}

		scriptable_object* get_caller() { return m_button; }


		EVENT_CLASS_TYPE(GUI_Transport_Select)
		EVENT_CLASS_CATEGORY(Event_Catagory_Menu)

	private:
		scriptable_object* m_button;
	};

	class GUI_Building_Size_Event : public Event
	{
	public:
		GUI_Building_Size_Event(scriptable_object* caller, scriptable_object* building_type)
		{
			m_button = caller;
			m_caller = building_type;
		}

		scriptable_object* get_caller() { return m_button; }
		scriptable_object* get_menu_caller() { return m_caller;  }

		EVENT_CLASS_TYPE(GUI_Size_Select)
		EVENT_CLASS_CATEGORY(Event_Catagory_Menu)

	private:
		scriptable_object* m_button;
		scriptable_object* m_caller;
	};

	class GUI_Editor_Event : public Event
	{
	public:
		GUI_Editor_Event()
		{
		}

		EVENT_CLASS_TYPE(GUI_Editor)
		EVENT_CLASS_CATEGORY(Event_Catagory_Menu)
	};
}