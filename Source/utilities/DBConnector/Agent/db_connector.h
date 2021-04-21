#pragma once

#include <filesystem>
#include <vector>

extern "C" {
    #include "..\..\..\..\include\SQLite\sqlite3.h"
};

#ifdef _WIN32
#else

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
        void handle_exception(std::exception& e);
};