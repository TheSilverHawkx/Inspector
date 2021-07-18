#include "monitor_block.h"

bool MonitorBlock::run_block() {
    try {
        return this->execute();
    }
    catch (std::exception& e){
        throw;
    }
};

rapidjson::Document* MonitorBlock::parse_json(const char* json) {
    rapidjson::Document* doc = new rapidjson::Document;
    doc->Parse(json);



    if (doc->HasParseError()) {
        std::string formatted_error = {typeid(this).name()};
        formatted_error.append(": parsing error at ");
        formatted_error.append(std::to_string(doc->GetErrorOffset()));
        formatted_error.append(std::to_string(json[doc->GetErrorOffset()]));

        formatted_error.append(": ");
        formatted_error.append(rapidjson::GetParseError_En(doc->GetParseError()));
        
        delete doc;
        throw std::runtime_error(formatted_error);
    }

    return doc;
}