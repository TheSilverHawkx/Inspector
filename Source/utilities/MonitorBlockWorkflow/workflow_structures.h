#pragma once

#include <sstream>
#ifdef _WIN32
    #include "..\..\Monitor-Blocks\monitor_block.h"
#else
    #include "../../Monitor-Blocks/monitor_block.h"
#endif

struct dispatcher_time_struct {
    unsigned int seconds;
    unsigned int minutes;
    unsigned int hours;
    unsigned int days;
    unsigned int day_of_week;
    unsigned int week_of_month;

    std::ostream& serialize(std::ostream& os) const {
        char nill = '\0';
        os.write((char*)&(this->seconds),sizeof(unsigned int));
        os.write((char*)&(this->minutes),sizeof(unsigned int));
        os.write((char*)&(this->hours),sizeof(unsigned int));
        os.write((char*)&(this->days),sizeof(unsigned int));
        os.write((char*)&(this->day_of_week),sizeof(unsigned int));
        os.write((char*)&(this->week_of_month),sizeof(unsigned int));
        os.write(&nill,1);

        return os;
    }

    std::istream& deserialize(std::istream& is) const {
        is.read((char*)&(this->seconds),sizeof(this->seconds));
        is.read((char*)&(this->minutes),sizeof(this->minutes));
        is.read((char*)&(this->hours),sizeof(this->hours));
        is.read((char*)&(this->days),sizeof(this->days));
        is.read((char*)&(this->day_of_week),sizeof(this->day_of_week));
        is.read((char*)&(this->week_of_month),sizeof(this->week_of_month));

        return is;
    }
};

struct dispatcher_entry {
    const unsigned char* trigger_id;
    const unsigned char* workflow_id;
    dispatcher_time_struct parameter;
};

struct workflow_item_struct {
    unsigned int id;
    const unsigned char* block_id;
    unsigned int next_id;

    _block_type block_type;
    const char* block_class;
    const char* parameters;

};