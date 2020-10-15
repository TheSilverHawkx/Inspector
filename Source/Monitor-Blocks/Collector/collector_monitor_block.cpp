#include "collector_monitor_block.h"

CollectorMonitorBlock::CollectorMonitorBlock(const char* id,
                            const char* name,
                            const char* parameters,
                            _output_type output_type): MonitorBlock(id,name,_block_type::collector,"s",_output_type::ClearText) {
    this->parameters = parameters;
    this->output_type = output_type;
};
CollectorMonitorBlock::~CollectorMonitorBlock() {};

bool CollectorMonitorBlock::execute() {};

rapidjson::Document CollectorMonitorBlock::parse_parameters() {
    rapidjson::Document doc;
    doc.Parse(this->parameters.c_str());
    return doc;
}

void CollectorMonitorBlock::handle_exceptions(const std::exception e) {};