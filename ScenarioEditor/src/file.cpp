#include "file.h"

file_data* file_data::s_instance = nullptr;

file_data::file_data()
{
    if (s_instance != nullptr)
    {
        Log::crit("File data already exists", __FILE__, __LINE__);
    }

    s_instance = this;
}

file_data::~file_data()
{
}
