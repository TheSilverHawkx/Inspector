#pragma once

#include <filesystem>
#include <vector>


#ifdef _WIN32
    extern "C" {
        #include "..\..\..\..\include\SQLite\sqlite3.h"
    };
    #include "..\..\MonitorBlockWorkflow\workflow_structures.h"
    #include "..\..\..\Monitor-Blocks\all_monitor_blocks.h"
    #include "..\..\..\..\include\rapidjson\writer.h"
    #include "..\..\..\..\include\rapidjson\stringbuffer.h"
#else
    extern "C" {
        #include "../../../../include/SQLite/sqlite3.h"
    };
    #include "../../MonitorBlockWorkflow/workflow_structures.h"
    #include "../../../../include/rapidjson/writer.h"
    #include "../../../../include/rapidjson/stringbuffer.h"
    #include "../../../Monitor-Blocks/all_monitor_blocks.h
#endif


class DBConnector {
    private:
        sqlite3* db_con;
        const char* user;
        const char* password;

        void create_db(const char* path);
        sqlite3_stmt* prepare_statement(const char* query);

    public:
        bool is_ready;

        DBConnector(const char*);
        ~DBConnector();

        std::vector<std::string> get(const char* statement);
        void set(std::vector<char>& statement);
        void handle_exception(std::exception& e);

        // Custom Insert Queries
        void add_dispatch_entry(const char* id,const char* workflow_id,std::vector<char>& parameters);
        void DBConnector::add_monitorblock_entry(const char* id,const char* workflow_id, MonitorBlock* block);

        // Custom Select Queries
        std::vector<dispatcher_entry> get_dispatch_entires();
        std::vector<workflow_item_struct>get_workflow_items(const char* id);
        
};