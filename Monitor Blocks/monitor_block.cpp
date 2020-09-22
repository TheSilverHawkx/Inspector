#include "monitor_block.h"

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

void MonitorBlock::run_block() {
    if (this->execute(&this->output)) {
        this->execution_status = 0;
    }
    else
    {
        this->execution_status = 1;
    }
    
};