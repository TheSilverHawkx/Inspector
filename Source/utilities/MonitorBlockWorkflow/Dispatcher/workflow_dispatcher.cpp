#include "workflow_dispatcher.h"

WorkflowDispatcher::WorkflowDispatcher(const char* path, DBConnector* connector) {
    this->db_con = connector;
    this->workflow_list_path = path;

    this->update_table();

    try {
        std::thread listener_thread(&WorkflowDispatcher::start,this);
        listener_thread.detach();
    }
    catch (std::exception& e) {
        throw;
    }
};

WorkflowDispatcher::~WorkflowDispatcher() {};

void WorkflowDispatcher::update_table() {
    try {
        this->db_con->get_dispatch_entires(this->workflows_table);
    }
    catch (std::exception& e) {
        std::string err_msg {"Could not update dispatch table. error: "};
        err_msg.append(e.what());
        
        ///% Log to file
    }
}

void WorkflowDispatcher::start() {
    do {
        time_t now = std::time(0);
        tm* time_struct = std::localtime(&now);
        for (std::vector<dispatcher_entry>::iterator it= this->workflows_table.begin(); it != this->workflows_table.end();++it){

            // Check week of month
            if (it->parameter.week_of_month == -1 || (time_struct->tm_mday / 7) == it->parameter.week_of_month){
                // Check day of week
                if (it->parameter.day_of_week == -1 ||(time_struct->tm_wday + 1) == it->parameter.day_of_week) {
                    // Check "every [] day"
                    if (it->parameter.days == -1 ||time_struct->tm_mday % it->parameter.days == 0) {
                        // Check "every [] hours"
                        if (it->parameter.hours == -1 ||time_struct->tm_hour % it->parameter.hours == 0) {
                            // Check "every [] minutes"
                            if (it->parameter.minutes == -1 ||time_struct->tm_min % it->parameter.minutes == 0) {
                                // Check "every [] seconds"
                                if (it->parameter.seconds == -1 || time_struct->tm_sec % it->parameter.seconds ==0) {
                                    std::thread workflow_thread(&WorkflowDispatcher::create_workflow,this,&(*it));
                                    workflow_thread.detach();
                                }
                            }
                        }
                    }
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    } while (true);
}

void WorkflowDispatcher::create_workflow(dispatcher_entry* entry) {

    //% Add DB fetch to get workflow data
    if (atoi((const char*)entry->workflow_id) != 1) 
        return;
        
    /*std::map<unsigned int,workflow_item_struct> workflow_items;
    workflow_items.insert(std::pair<unsigned int,workflow_item_struct>(1,workflow_item_struct{"1","1",2,_block_type::collector,"CommandBlock"}));
    workflow_items.insert(std::pair<unsigned int,workflow_item_struct>(2,workflow_item_struct{"2","1",0,_block_type::condition,"SimpleEvaluationBlock"}));*/

    /*try {
        WorkflowManager* manager = new WorkflowManager("1",workflow_items);
        manager->run_workflow();

        delete manager;
    }
    catch (std::exception& e) {
        // write dispatcher errors regarding threads
    }*/

    
}

void WorkflowDispatcher::add_entry(dispatcher_entry& entry) {
    std::stringstream parameter;
    entry.parameter.serialize(parameter);

    //std::string statement {"INSERT INTO DISPATCH_TABLE(ID,Workflow_ID,Parameters) VALUES (\"one\",\"one\",\"" + parameter.str() + std::string("\");")};
    //this->db_con->set(parameter.str().c_str());
}