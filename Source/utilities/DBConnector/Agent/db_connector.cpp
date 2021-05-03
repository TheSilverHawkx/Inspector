#include "db_connector.h"

DBConnector::DBConnector(const char* db_file_path){
    this->user = "inspector-agent";
    this->password= "inspector-agent";

    try {
        // Check if DB exists
        if (!std::filesystem::exists(db_file_path)) {
            this->create_db(db_file_path);
        }
        else {
            if (sqlite3_open_v2(db_file_path,&(this->db_con),SQLITE_OPEN_READWRITE,NULL)) {
                std::string error_msg {"Failed to connect to DB. Details: "};
                error_msg.append(sqlite3_errmsg(this->db_con));
                throw error_msg;
            }

            if (sqlite3_user_authenticate(this->db_con,this->user,this->password,(int)strlen(this->password))) {
                std::string error_msg {"Failed to authenticate against DB. Details: "};
                error_msg.append(sqlite3_errmsg(this->db_con));
                throw error_msg;
            }
        }

        this->is_ready = true;
    }
    catch (std::exception& e) {
        handle_exception(e);
        return;
    }
}

DBConnector::~DBConnector(){
    sqlite3_close_v2(this->db_con);
}

void DBConnector::create_db(const char* db_file_path){

    // Create DB
    if (sqlite3_open_v2(db_file_path,&(this->db_con),SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,NULL)) {
        this->is_ready = false;
        throw std::runtime_error(std::string("DB Creation error: ") + sqlite3_errmsg(this->db_con));
    }

    // Create User
    if (sqlite3_user_add(this->db_con,this->user,this->password,(int)strlen(this->password),true)) {
        this->is_ready = false;

        std::string error_msg {"Failed to create user. Details: "};
        error_msg.append(sqlite3_errmsg(this->db_con));
        throw error_msg;
    }
}

void DBConnector::handle_exception(std::exception& e) {
    sqlite3_close_v2(this->db_con);
}

sqlite3_stmt* DBConnector::prepare_statement(const char* query) {
    sqlite3_stmt* preped_statement;

    if (sqlite3_prepare_v2(this->db_con,query,(int)strlen(query),&preped_statement,NULL)) {
        std::string error_msg {"Failed preparing query statement. Query: \""+ std::string(query) + "\" Details: "};
        error_msg.append(sqlite3_errmsg(this->db_con));
        throw error_msg;
    }

    return preped_statement;
}

std::vector<std::string> DBConnector::get(const char* statement) {
    int rc;
    sqlite3_stmt* preped_statement = this->prepare_statement(statement);

    std::vector<std::string> result;

    rc = sqlite3_step(preped_statement);
    if (rc == SQLITE_ROW) {
        int column_count = sqlite3_column_count(preped_statement);

        while ( rc != SQLITE_DONE) {
            if (rc != SQLITE_DONE && rc != SQLITE_ROW) {
                std::string error_msg {"Failed running query statement. Query: \""+ std::string(statement) + "\" Details: "};
                error_msg.append(sqlite3_errmsg(this->db_con));
                throw error_msg;
            }

            for (int i = 0; i < column_count; i++) {
                if (sqlite3_column_type(preped_statement,i) != SQLITE_NULL)
                    result.push_back(reinterpret_cast<const char*>(sqlite3_column_text(preped_statement,i)));
            }
            rc = sqlite3_step(preped_statement);
        }
    }
    else if (rc != SQLITE_DONE) {
        std::string error_msg {"Failed running query statement. Query: \""+ std::string(statement) + "\" Details: "};
        error_msg.append(sqlite3_errmsg(this->db_con));
        throw error_msg;
    }

    sqlite3_finalize(preped_statement);

    return result;
}
