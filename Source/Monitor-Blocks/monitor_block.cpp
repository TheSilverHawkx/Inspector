#include "monitor_block.h"
#include <iostream>

MonitorBlock::MonitorBlock(const char* id,
                            const char* name,
                            _block_type type,
                            const char* parameters,
                            _output_type output_type) {
    this->id = id;
    this->name = name;
    this->block_type = type;
    this->parameters = parameters;
    this->output_type = output_type;
};
MonitorBlock::~MonitorBlock() {};

void MonitorBlock::run_block() {
    if (this->execute()) {
        this->execution_status = 0;
    }
    else
    {
        this->execution_status = 1;
    }
    
    std::cout << "execution code: " << this->execution_status << std::endl;
};

bool MonitorBlock::execute() {
    return false;
};

rapidjson::Document MonitorBlock::parse_parameters() {
    rapidjson::Document doc;
    doc.Parse(this->parameters.c_str());
    return doc;
}

void MonitorBlock::handle_exceptions(const std::exception e) {
    
};