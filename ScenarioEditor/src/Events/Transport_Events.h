#pragma once

#include "Event.h"

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


};