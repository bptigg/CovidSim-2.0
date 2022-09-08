#pragma once

#include "Event.h"
#include "../Entity/scriptable_object.h"

namespace Events
{

	class Transport_Overlay_Event : public Event
	{
	public:
		Transport_Overlay_Event(bool enable)
		{
			m_enable = enable;
		}

		const bool get_enable() const { return m_enable; }

		EVENT_CLASS_TYPE(Transport_Overlay_Select)
		EVENT_CLASS_CATEGORY(Event_Catagory_Transport)
	private:
		bool m_enable;
	};

	class Transport_stop_select : public Event
	{
	public:
		Transport_stop_select(scriptable_object* caller)
		{
			m_caller = caller;
		}

		scriptable_object* get_caller() { return m_caller; }

		EVENT_CLASS_TYPE(Transport_Stop_Select)
		EVENT_CLASS_CATEGORY(Event_Catagory_Transport)
	private:
		scriptable_object* m_caller;
	};


};