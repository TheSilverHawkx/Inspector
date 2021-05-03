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

WorkflowDispatcher::~WorkflowDispatcher() {
};

void WorkflowDispatcher::update_table() {
    try {
        this->workflows_table = this->db_con->get_dispatch_entires();
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
            if (it->time_struct.week_of_month == 999 || (time_struct->tm_mday / 7) == (int)it->time_struct.week_of_month){
                // Check day of week
                if (it->time_struct.day_of_week == 999 ||(time_struct->tm_wday + 1) == (int)it->time_struct.day_of_week) {
                    // Check "every [] day"
                    if (it->time_struct.days == 999 ||time_struct->tm_mday % it->time_struct.days == 0) {
                        // Check "every [] hours"
                        if (it->time_struct.hours == 999 ||time_struct->tm_hour % it->time_struct.hours == 0) {
                            // Check "every [] minutes"
                            if (it->time_struct.minutes == 999 ||time_struct->tm_min % it->time_struct.minutes == 0) {
                                // Check "every [] seconds"
                                if (it->time_struct.seconds == 999 || time_struct->tm_sec % it->time_struct.seconds ==0) {
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

    std::map<unsigned int,workflow_item_struct> workflow_items;
    std::vector<workflow_item_struct> items = db_con->get_workflow_items(entry->workflow_id.c_str());
    for (std::vector<workflow_item_struct>::iterator it = items.begin(); it != items.end(); ++it) {
        workflow_items[it->id] = *it;
    }

    WorkflowManager* manager = new WorkflowManager(entry->workflow_id.c_str(),workflow_items);
    try {
        manager->run_workflow();


        // Handle writing to log about finished workflow
    }
    catch (std::exception& e) {
        // Handle writing to log about error in workflow
        std::string err_msg {"Workflow (" + entry->workflow_id + ") failed. Details: "};
        err_msg.append(e.what());

    }

    delete manager;
}

void WorkflowDispatcher::add_entry(dispatcher_entry& entry) {
    std::stringstream parameter;
    entry.time_struct.serialize(parameter);
}