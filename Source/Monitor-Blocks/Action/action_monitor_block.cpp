#include "action_monitor_block.h"

ActionMonitorBlock::ActionMonitorBlock(const char* id,const char* parameters,MonitorBlock* block) {
    this->id = id;
    this->parameters = this->parse_json(parameters);
    this->output_type = _output_type::Action;

    if (block == nullptr) {
        throw inspector::MonitorBlockException(this->id.c_str(),"Received null collector block pointer.");
    }
    
    if (block->block_type == _block_type::collector) {
        if (CommandMonitorBlock* casted_collector = dynamic_cast<CommandMonitorBlock*>(block)) {
            this->collected_data = casted_collector->output->to_list();
        }
        else if (ScriptMonitorBlock* casted_collector = dynamic_cast<ScriptMonitorBlock*>(block)) {
            this->collected_data = casted_collector->output->to_list();
        }
        #ifdef _WIN32
        else if (WMIMonitorBlock* casted_collector = dynamic_cast<WMIMonitorBlock*>(block)) {
            this->collected_data = casted_collector->output->to_list();
        }
        #endif
    }
    else if (block->block_type == _block_type::condition) {
        if (CommandMonitorBlock* casted_collector = dynamic_cast<CommandMonitorBlock*>(block)) {
            this->collected_data = casted_collector->output->to_list();
        }
    }
    else {
        throw inspector::MonitorBlockException(this->id.c_str(),"Unsupported collector block type received.");
    }
};

ActionMonitorBlock::~ActionMonitorBlock() { delete this->parameters; };

bool ActionMonitorBlock::execute() { return false; };

void ActionMonitorBlock::handle_exceptions(const std::exception e) {};

std::string ActionMonitorBlock::format_text() {
    if (!(*this->parameters)["text"].HasMember("template")) {
        return "";
    }

    std::smatch matches;
    std::string text = (*this->parameters)["text"]["template"].GetString();
    std::string formatted_text {};

    for (auto& custom_part : (*this->parameters)["text"]["custom_parts"].GetArray()) {

        std::string string_expression {"\\[var-"};
        string_expression.append(custom_part["name"].GetString());
        string_expression.append("\\]");

        std::regex expression(string_expression);

        formatted_text = std::regex_replace(text,expression,this->collected_data[custom_part["value"].GetInt()]);
        text = formatted_text;
    }
    return formatted_text;
}