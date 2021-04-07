#include "simple-evaluation_block.h"

SimpleEvaluationBlock::SimpleEvaluationBlock(const char* id,const char* parameters,CollectorMonitorBlock* collector) :
ConditionalMonitorBlock(id,parameters,collector) {};

SimpleEvaluationBlock::~SimpleEvaluationBlock() {};

bool SimpleEvaluationBlock::execute() {
    try {
        rapidjson::Value& condition_list = this->parameters;
        return inspector::evaluate_condition(condition_list,this->collected_data);
    }
    catch (std::exception e)
    {
        throw;
    }
}

void SimpleEvaluationBlock::handle_exceptions(const std::exception e) {
    std::string caught_exception = e.what();
    std::vector<std::vector<std::string>> error_vector;
    std::vector<std::string> error_line {"WMI execution failure: " + caught_exception};
    error_vector.push_back(error_line);
    this->output->data = &error_vector;
    this->output->return_code = -1;
};