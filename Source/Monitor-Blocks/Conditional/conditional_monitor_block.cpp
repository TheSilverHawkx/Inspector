#include "conditional_monitor_block.h"

ConditionalMonitorBlock::ConditionalMonitorBlock(const char* id,const char* parameters,CollectorMonitorBlock* collector) {
    this->id = id;
    this->name = "";
    this->parameters.Parse(parameters);
    this->output_type = output_type;

    if (CommandMonitorBlock* casted_collector = dynamic_cast<CommandMonitorBlock*>(collector)) {
        collected_data = casted_collector->output->to_list();
    }
    else if (ScriptMonitorBlock* casted_collector = dynamic_cast<ScriptMonitorBlock*>(collector)) {
        collected_data = casted_collector->output->to_list();
    }
    #ifdef _WIN32
    else if (WMIMonitorBlock* casted_collector = dynamic_cast<WMIMonitorBlock*>(collector)) {
        collected_data = casted_collector->output->to_list();
    }
    #endif
    else {
        throw std::invalid_argument("Unsupported collector block type received in conditional block.");
    }

};


ConditionalMonitorBlock::~ConditionalMonitorBlock() {};

bool ConditionalMonitorBlock::execute() {
    return false;
};

void ConditionalMonitorBlock::handle_exceptions(const std::exception e) {};