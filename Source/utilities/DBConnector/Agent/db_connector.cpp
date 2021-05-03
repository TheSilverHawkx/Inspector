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


void DBConnector::set(std::vector<char>& statement) {
    int rc;
    char* err_msg {0};

    rc = sqlite3_exec(this->db_con,"BEGIN TRANSACTION;",NULL,NULL,&err_msg);

    if (rc) {
        throw std::runtime_error("Error commiting to db. Details: " + std::string(err_msg));
    }

    sqlite3_stmt* stm {nullptr};
    
    rc = sqlite3_prepare_v2(this->db_con,"INSERT INTO DISPATCH_TABLE(ID,Workflow_ID,Parameters) VALUES (\"two\",\"two\",?);",-1,&stm,NULL);
    //rc = sqlite3_prepare_v2(this->db_con,"update Monitor_Blocks SET Parameters = ? WHERE ID = 'bash_script';",-1,&stm,NULL);

    if (rc) {
        //const char* err_msg1 = sqlite3_errmsg(this->db_con);
        throw std::runtime_error("Error preparing commit statement. Details: "+ std::string(sqlite3_errmsg(this->db_con)));
    }
    
    rc = sqlite3_bind_blob(stm,1,statement.data(),statement.size(),SQLITE_STATIC);

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

std::vector<dispatcher_entry> DBConnector::get_dispatch_entires() {
    std::vector<dispatcher_entry> entries;
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

        entry.trigger_id = (const char*)sqlite3_column_text(preped_statement,0);
        entry.workflow_id= (const char*)sqlite3_column_text(preped_statement,1);
        
        std::stringstream ss;
        ss.write((char *)sqlite3_column_blob(preped_statement,2),sqlite3_column_bytes(preped_statement,2));
        entry.time_struct.deserialize(ss);

        entries.push_back(entry);
        rc = sqlite3_step(preped_statement);
    }

    sqlite3_finalize(preped_statement);

    return entries;
}

std::vector<workflow_item_struct> DBConnector::get_workflow_items(const char* id) {
    std::vector<workflow_item_struct> items;
    
    int rc;

    sqlite3_stmt* preped_statement = this->prepare_statement("SELECT wi.id,wi.Block_ID,wi.Next_Item_ID,mb.Block_Type,mb.Block_Class,mb.Parameters FROM  Workflow_Items wi INNER JOIN Monitor_Blocks mb on mb.ID= wi.Block_ID WHERE wi.Workflow_ID = ?;");

    if (sqlite3_bind_text(preped_statement,1,id,strlen(id),SQLITE_STATIC)){
        throw std::runtime_error("Error binding id. Details: " + std::string(sqlite3_errmsg(this->db_con)));
    }

    rc = sqlite3_step(preped_statement);
    if (rc != SQLITE_ROW && rc != SQLITE_DONE) {
        std::string error_msg {"Failed Fetching entries from workflow items. Details: "};
        error_msg.append(sqlite3_errmsg(this->db_con));
        throw error_msg;
    }
    
    while ( rc != SQLITE_DONE) {
        workflow_item_struct item;

        item.id = sqlite3_column_int(preped_statement,0);
        item.block_id = (const char*)sqlite3_column_text(preped_statement,1);
        item.next_id =  sqlite3_column_int(preped_statement,2);
        item.block_type = (_block_type)sqlite3_column_int(preped_statement,3);
        item.block_class = (const char*)(sqlite3_column_text(preped_statement,4));
        item.parameters = (const char*)(sqlite3_column_blob(preped_statement,5));

        items.push_back(item);
        rc = sqlite3_step(preped_statement);
    }

    sqlite3_finalize(preped_statement);
    return items;
}

void DBConnector::add_dispatch_entry(const char* id,const char* workflow_id,std::vector<char>& parameters) {
    int rc;
    char* err_msg {0};

    rc = sqlite3_exec(this->db_con,"BEGIN TRANSACTION;",NULL,NULL,&err_msg);

    if (rc) {
        throw std::runtime_error("Error commiting to db. Details: " + std::string(err_msg));
    }

    sqlite3_stmt* stm {nullptr};
    
    std::string statement {"INSERT INTO DISPATCH_TABLE(ID,Workflow_ID,Parameters) VALUES (?,?,?);"};

    if (sqlite3_prepare_v2(this->db_con,statement.c_str(),-1,&stm,NULL)) {
        throw std::runtime_error("Error preparing commit statement. Details: "+ std::string(sqlite3_errmsg(this->db_con)));
    }
    
    if (sqlite3_bind_text(stm,1,id,strlen(id),SQLITE_STATIC)) {
        throw std::runtime_error("Error binding id. Details: " + std::string(sqlite3_errmsg(this->db_con)));
    }
    if (sqlite3_bind_blob(stm,2,workflow_id,strlen(workflow_id),SQLITE_STATIC)) {
        throw std::runtime_error("Error binding workflow_id. Details: " + std::string(sqlite3_errmsg(this->db_con)));
    }
    if (sqlite3_bind_blob(stm,3,parameters.data(),parameters.size(),SQLITE_STATIC)) {
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

void DBConnector::add_monitorblock_entry(const char* id,const char* workflow_id, MonitorBlock* block) {
    int rc;
    char* err_msg {0};
    std::string class_name {};
    std::string parameters {};
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

    if (CommandMonitorBlock* casted_block = dynamic_cast<CommandMonitorBlock*>(block)) {
        class_name = "CommandMonitorBlock";
        casted_block->get_parameters()->Accept(writer);
        parameters = buffer.GetString();
    }
    else if (ScriptMonitorBlock* casted_block = dynamic_cast<ScriptMonitorBlock*>(block)) {
        class_name = "ScriptMonitorBlock";
        casted_block->get_parameters()->Accept(writer);
        parameters = buffer.GetString();
    }
    #ifdef _WIN32
    else if (WMIMonitorBlock* casted_block = dynamic_cast<WMIMonitorBlock*>(block)) {
        class_name = "WMIMonitorBlock";
        casted_block->get_parameters()->Accept(writer);
        parameters = buffer.GetString();
    }
    #endif

    rc = sqlite3_exec(this->db_con,"BEGIN TRANSACTION;",NULL,NULL,&err_msg);

    if (rc) {
        throw std::runtime_error("Error commiting to db. Details: " + std::string(err_msg));
    }

    sqlite3_stmt* stm {nullptr};
    
    std::string statement {"INSERT INTO Monitor_Blocks(ID,Workflow_ID,Block_Type,Block_Class,Parameters) VALUES (?,?,?,?,?);"};

    if (sqlite3_prepare_v2(this->db_con,statement.c_str(),-1,&stm,NULL)) {
        throw std::runtime_error("Error preparing commit statement. Details: "+ std::string(sqlite3_errmsg(this->db_con)));
    }
    if (sqlite3_bind_text(stm,1,id,strlen(id),SQLITE_STATIC)) {
        throw std::runtime_error("Error binding id. Details: " + std::string(sqlite3_errmsg(this->db_con)));
    }
    if (sqlite3_bind_text(stm,2,workflow_id,strlen(workflow_id),SQLITE_STATIC)) {
        throw std::runtime_error("Error binding workflow_id. Details: " + std::string(sqlite3_errmsg(this->db_con)));
    }
    if (sqlite3_bind_int(stm,3,(int)block->block_type)) {
        throw std::runtime_error("Error binding blob. Details: " + std::string(sqlite3_errmsg(this->db_con)));
    }
    if (sqlite3_bind_text(stm,4,class_name.c_str(),class_name.size(),SQLITE_STATIC)) {
        throw std::runtime_error("Error binding blob. Details: " + std::string(sqlite3_errmsg(this->db_con)));
    }
    if (sqlite3_bind_blob(stm,5,parameters.data(),parameters.size(),SQLITE_STATIC)) {
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