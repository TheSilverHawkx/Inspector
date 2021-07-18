#include "create_alert_block.h"

CreateAlertBlock::CreateAlertBlock(const char* id,const char* parameters,MonitorBlock* block) :
ActionMonitorBlock(id,parameters,block) {
    this->severity = (_severity)(*this->parameters)["severity"].GetInt();
    this->alert_name = (*this->parameters)["alert_name"].GetString();
};

CreateAlertBlock::~CreateAlertBlock() {};

bool CreateAlertBlock::execute() {
    try {
        std::string additional_data = this->serialize_collected_data();
        std::string formatted_text =this->format_text();
        


        return true;
    }
    catch (const std::exception& e)
    {
        throw;
    }
};

void CreateAlertBlock::handle_exceptions(const std::exception e) {
    std::string caught_exception = e.what();
    std::vector<std::vector<std::string>> error_vector;
    throw std::runtime_error("Create Alert: " + caught_exception);
};

std::string CreateAlertBlock::serialize_collected_data() {
    std::string serialized_data {};

    for (std::vector<std::string>::iterator it = this->collected_data.begin(); it != this->collected_data.end();) {
        serialized_data.append((*it));
        if (++it != this->collected_data.end())
            serialized_data.append("\n");
    }
    
    return serialized_data;
}