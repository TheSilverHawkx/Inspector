#include "log_to_file_block.h"

LogToFileBlock::LogToFileBlock(const char* id,const char* parameters,MonitorBlock* block) :
ActionMonitorBlock(id,parameters,block) {
    int mode =(*this->parameters)["mode"].GetInt();

    if ((*this->parameters)["file_path"] == "") {
        // Append folder prefix of default log
        std::string default_file_path = "workdir\\";
        default_file_path.append("shoop");

        // Create a folder if needed
        if (!std::filesystem::exists(default_file_path)) {
            std::filesystem::create_directory(default_file_path);
        }

        // Append suffix with default log name
        default_file_path.append("\\monitor.log");

        initialize_file(default_file_path,(_log_mode)mode);
    } else {
        initialize_file((std::string)(*this->parameters)["file_path"].GetString(),(_log_mode)mode);
    }
};

LogToFileBlock::~LogToFileBlock() {};

bool LogToFileBlock::execute() {
    try {
        std::string formatted_text =this->format_text();
        this->file << formatted_text;
        this->file.close();
        return true;
    }
    catch (const std::exception& e)
    {
        throw;
    }
};

void LogToFileBlock::handle_exceptions(const std::exception e) {
    std::string caught_exception = e.what();
    std::vector<std::vector<std::string>> error_vector;
    throw std::runtime_error("LogToFile: " + caught_exception);
};

void LogToFileBlock::initialize_file(std::string path, _log_mode mode) {
    switch (mode)
    {
        case _log_mode::overwrite:
            this->file.open(path,std::ios::out | std::ios::trunc);
            break;
        case _log_mode::binary:
            this->file.open(path,std::ios::out | std::ios::binary);
            break;
        case _log_mode::append:
            this->file.open(path,std::ios::out | std::ios::app);
            break;
        default:
            throw inspector::MonitorBlockException(this->id.c_str(),"Unknown write mode found.");
            break;
    }
}