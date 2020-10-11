#include "schedule_block.h"


ScheduleMonitorBlock::ScheduleMonitorBlock(const char* id,const char* name,const char* parameters) :
MonitorBlock(id,name,_block_type::trigger,parameters,_output_type::Trigger) {};

ScheduleMonitorBlock::~ScheduleMonitorBlock() {};

bool ScheduleMonitorBlock::execute() {

};

void ScheduleMonitorBlock::handle_exceptions(const std::exception e) {
    std::string caught_exception = e.what();
    //*(this->output->data) = "Command execution failure: " + caught_exception;
    //this->output->return_code = -1;
};