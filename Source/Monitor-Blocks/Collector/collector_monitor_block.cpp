#include "collector_monitor_block.h"

CollectorMonitorBlock::CollectorMonitorBlock(const char* id,
                            const char* name,
                            const char* parameters,
                            _output_type output_type) {
    this->id = id;
    this->name = name;
    this->parameters.Parse(parameters);
    this->output_type = output_type;
};
CollectorMonitorBlock::~CollectorMonitorBlock() {};

bool CollectorMonitorBlock::execute() {
    return false;
};

void CollectorMonitorBlock::handle_exceptions(const std::exception e) {};