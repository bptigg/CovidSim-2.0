#pragma once

#include "Event.h"

#include "../Entity/scriptable_object.h"
#include "../Layers/Transport_Layer.h"

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

	class GUI_Settings_Event : public Event
	{
	public:
		GUI_Settings_Event(scriptable_object* caller)
		{
			m_button = caller;
		}

		scriptable_object* get_caller() { return m_button; }


		EVENT_CLASS_TYPE(GUI_Settings_Select)
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
		GUI_Transport_Building_Event(scriptable_object* caller, scriptable_object* menu)
		{
			m_button = caller;
			m_menu = menu;
		}

		scriptable_object* get_caller() { return m_button; }
		scriptable_object* get_menu() { return m_menu; }


		EVENT_CLASS_TYPE(GUI_Transport_Select)
		EVENT_CLASS_CATEGORY(Event_Catagory_Menu)

	private:
		scriptable_object* m_button;
		scriptable_object* m_menu;
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

	class GUI_Line_Manager_Event : public Event
	{
	public:
		GUI_Line_Manager_Event(Transport_Layer* layer)
		{
			m_layer = layer;
		}

		Transport_Layer* get_layer() { return m_layer; }

		EVENT_CLASS_TYPE(Transport_Line_Manager)
		EVENT_CLASS_CATEGORY(Event_Catagory_Menu)

	private:
		Transport_Layer* m_layer;
	};

	class GUI_Line_Editor_Event : public Event
	{
	public:
		GUI_Line_Editor_Event(bool open)
		{
			m_active = open;
			m_layer = nullptr;
		}

		void add_layer(Transport_Layer* layer) { m_layer = layer; }
		Transport_Layer* get_layer() { return m_layer; }
		bool get_active() { return m_active; }

		EVENT_CLASS_TYPE(Transport_Line_Editor)
		EVENT_CLASS_CATEGORY(Event_Catagory_Menu)

	private:
		Transport_Layer* m_layer;
		bool m_active;
	};

	class GUI_Dropdown_Event : public Event
	{
	public:
		GUI_Dropdown_Event(scriptable_object* caller)
		{
			m_caller = caller;
		}

		scriptable_object* get_caller() { return m_caller; }

		EVENT_CLASS_TYPE(GUI_Dropdown)
		EVENT_CLASS_CATEGORY(Event_Catagory_Menu)

	private:
		scriptable_object* m_caller;
	};

	class Popup_Staff_Event : public Event
	{
	public:
		Popup_Staff_Event(scriptable_object* caller)
		{
			m_caller = caller;
		}

		scriptable_object* get_caller() { return m_caller; }

		EVENT_CLASS_TYPE(Popup_Staff)
		EVENT_CLASS_CATEGORY(Event_Catagory_Popup)
	private:
		scriptable_object* m_caller;
	};

	class Popup_Capacity_Event : public Event
	{
	public:
		Popup_Capacity_Event(scriptable_object* caller)
		{
			m_caller = caller;
		}

		scriptable_object* get_caller() { return m_caller; }

		EVENT_CLASS_TYPE(Popup_Capacity)
		EVENT_CLASS_CATEGORY(Event_Catagory_Popup)
	private:
		scriptable_object* m_caller;
	};

	class Popup_Opening_Event : public Event
	{
	public:
		Popup_Opening_Event(scriptable_object* caller)
		{
			m_caller = caller;
		}

		scriptable_object* get_caller() { return m_caller; }

		EVENT_CLASS_TYPE(Popup_Opening)
		EVENT_CLASS_CATEGORY(Event_Catagory_Popup)
	private:
		scriptable_object* m_caller;
	};
}