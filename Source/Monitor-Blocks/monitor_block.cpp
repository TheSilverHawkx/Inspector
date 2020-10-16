#include "monitor_block.h"

void MonitorBlock::run_block() {
    if (this->execute()) {
        this->execution_status = 0;
    }
    else
    {
        this->execution_status = 1;
    }
};
/*
bool MonitorBlock::execute() {
    return false;
};

rapidjson::Document MonitorBlock::parse_parameters() {
    rapidjson::Document doc;
    doc.Parse(this->parameters.c_str());
    return doc;
}

void MonitorBlock::handle_exceptions(const std::exception e) {
    
};*/