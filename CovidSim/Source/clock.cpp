#include "clock.h"

world_clock::world_clock()
{
    m_day_count = 0;
    m_day = CONSTANTS::DAY_OF_THE_WEEK::MONDAY;
}

bool world_clock::update_clock()
{
    m_day_count++;
    if (m_day_count > CONSTANTS::DAY_LENGTH)
    {
        m_day_count = 0;

        switch (m_day)
        {
        case CONSTANTS::DAY_OF_THE_WEEK::MONDAY:
            m_day = CONSTANTS::DAY_OF_THE_WEEK::TUESDAY;
            break;
        case CONSTANTS::DAY_OF_THE_WEEK::TUESDAY:
            m_day = CONSTANTS::DAY_OF_THE_WEEK::WEDNESDAY;
            break;
        case CONSTANTS::DAY_OF_THE_WEEK::WEDNESDAY:
            m_day = CONSTANTS::DAY_OF_THE_WEEK::THURSDAY;
            break;
        case CONSTANTS::DAY_OF_THE_WEEK::THURSDAY:
            m_day = CONSTANTS::DAY_OF_THE_WEEK::FRIDAY;
            break;
        case CONSTANTS::DAY_OF_THE_WEEK::FRIDAY:
            m_day = CONSTANTS::DAY_OF_THE_WEEK::SATURDAY;
            break;
        case CONSTANTS::DAY_OF_THE_WEEK::SATURDAY:
            m_day = CONSTANTS::DAY_OF_THE_WEEK::SUNDAY;
            break;
        case CONSTANTS::DAY_OF_THE_WEEK::SUNDAY:
            m_day = CONSTANTS::DAY_OF_THE_WEEK::MONDAY;
            break;
        default:
            break;
        }

        return true;
    }
    return false;
}

void world_clock::set_up_clock(CONSTANTS::DAY_OF_THE_WEEK Day)
{
    m_day = Day;
    m_day_count = 0;
}
