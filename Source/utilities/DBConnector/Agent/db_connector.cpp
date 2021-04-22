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


void DBConnector::set(const char* statement) {
    int rc;
    char* err_msg {0};

    rc = sqlite3_exec(this->db_con,"BEGIN TRANSACTION;",NULL,NULL,&err_msg);

    if (rc) {
        throw std::runtime_error("Error commiting to db. Details: " + std::string(err_msg));
    }

    sqlite3_stmt* stm {nullptr};
    
    //rc = sqlite3_prepare_v2(this->db_con,"INSERT INTO DISPATCH_TABLE(ID,Workflow_ID,Parameters) VALUES (\"one\",\"one\",?);",-1,&stm,NULL);
    rc = sqlite3_prepare_v2(this->db_con,"update Monitor_Blocks SET Parameters = ? WHERE ID = 'bash_script';",-1,&stm,NULL);

    if (rc) {
        const char* err_msg1 = sqlite3_errmsg(this->db_con);
        throw std::runtime_error("Error preparing commit statement. Details: "+ std::string(sqlite3_errmsg(this->db_con)));
    }
    
    rc = sqlite3_bind_blob(stm,1,statement,strlen(statement),SQLITE_STATIC);

    if (rc) {
        throw std::runtime_error("Error binding blob. Details: " + std::string(sqlite3_errmsg(this->db_con)));
    }

    rc = sqlite3_step(stm);

    if (rc != SQLITE_DONE) {
        throw std::runtime_error("Error commiting to db. Details: " + std::string(err_msg));
    }
    sqlite3_finalize(stm);

    rc = sqlite3_exec(this->db_con,"END TRANSACTION;",NULL,NULL,&err_msg);

    if (rc) {
        throw std::runtime_error("Error commiting to db. Details: " + std::string(err_msg));
    }

}

void DBConnector::get_dispatch_entires(std::vector<dispatcher_entry>& entries) {
    int rc;
    sqlite3_stmt* preped_statement = this->prepare_statement("SELECT ID,Workflow_ID,Parameters FROM DISPATCH_TABLE;");


    rc = sqlite3_step(preped_statement);
    if (rc != SQLITE_ROW && rc != SQLITE_DONE) {
        std::string error_msg {"Failed Fetching entries from dispatch table. Details: "};
        error_msg.append(sqlite3_errmsg(this->db_con));
        throw error_msg;
    }
    
    while ( rc != SQLITE_DONE) {
        dispatcher_entry entry;
        dispatcher_time_struct time_struct;

        entry.trigger_id = sqlite3_column_text(preped_statement,0);
        entry.workflow_id = sqlite3_column_text(preped_statement,1);
        const char* blob_content = static_cast<const char*>(sqlite3_column_blob(preped_statement,2));

        std::stringstream ss;
        ss.str(blob_content);
        time_struct.deserialize(ss);

        entry.parameter = time_struct;
        entries.push_back(entry);

        rc = sqlite3_step(preped_statement);
    }

    sqlite3_finalize(preped_statement);
}

std::vector<workflow_item_struct> DBConnector::get_workflow_items(const char* id) {
    std::vector<workflow_item_struct> items;
    
    int rc;

    std::string query {"SELECT wi.id,wi.Block_ID,wi.Next_Item_ID,mb.Block_Type,mb.Block_Class,mb.Parameters FROM  Workflow_Items wi INNER JOIN Monitor_Blocks mb on mb.ID= wi.Block_ID WHERE wi.Workflow_ID = "};
    query.append(id).append(";");

    sqlite3_stmt* preped_statement = this->prepare_statement(query.c_str());

    rc = sqlite3_step(preped_statement);
    if (rc != SQLITE_ROW && rc != SQLITE_DONE) {
        std::string error_msg {"Failed Fetching entries from workflow items. Details: "};
        error_msg.append(sqlite3_errmsg(this->db_con));
        throw error_msg;
    }
    
    while ( rc != SQLITE_DONE) {
        workflow_item_struct item;

        item.id = sqlite3_column_int(preped_statement,0);
        item.block_id = sqlite3_column_text(preped_statement,1);
        item.next_id =  sqlite3_column_int(preped_statement,2);
        item.block_type = (_block_type)sqlite3_column_int(preped_statement,3);
        item.block_class = reinterpret_cast<const char*>(sqlite3_column_text(preped_statement,4));
        item.parameters = reinterpret_cast<const char*>(sqlite3_column_blob(preped_statement,5));

        rc = sqlite3_step(preped_statement);
    }

    sqlite3_finalize(preped_statement);
    return items;
}

