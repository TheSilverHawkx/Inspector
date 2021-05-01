#include "collector_monitor_block.h"

CollectorMonitorBlock::CollectorMonitorBlock(const char* id,
                            const char* name,
                            const char* parameters,
                            _output_type output_type) {
    this->id = id;
    this->name = name;
    this->parameters = this->parse_json(parameters);
    this->output_type = output_type;
};
CollectorMonitorBlock::~CollectorMonitorBlock() {
    delete this->parameters;
};

bool CollectorMonitorBlock::execute() {
    return false;
};

void CollectorMonitorBlock::handle_exceptions(const std::exception e) {};

rapidjson::Document* CollectorMonitorBlock::get_parameters() {
    return this->parameters;
}