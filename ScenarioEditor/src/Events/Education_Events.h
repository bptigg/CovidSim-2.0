#pragma once

#include "Event.h"

namespace Events
{
	class Education_overlay_select : public Event
	{
	public:
		Education_overlay_select(bool enable)
		{
			m_enable = enable;
		}

		bool get_enable() { return m_enable; }

		EVENT_CLASS_TYPE(Education_Overlay_Select)
		EVENT_CLASS_CATEGORY(Event_Catagory_Education)
	private:
		bool m_enable;
	};
}
