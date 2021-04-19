#include "workflow_dispatcher.h"

WorkflowDispatcher::WorkflowDispatcher(const char* path) {
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

    std::ifstream table(this->workflow_list_path);

    unsigned int id,type,workflow_id;
    std::string parameter;

    while (table >> id >> type >> parameter >> workflow_id)
    {
        this->workflows_table.push_back(dispatcher_entry {id,type,parameter,workflow_id});
    }
    
    table.close();
}

void WorkflowDispatcher::start() {
    
    do {
        time_t now = std::time(0);
        tm* time_struct = std::localtime(&now);
        for (std::vector<dispatcher_entry>::iterator it= this->workflows_table.begin(); it != this->workflows_table.end();++it){

            // Handle regular seconds interval
            switch (it->trigger_type) {
                case 1: {
                    if (time_struct->tm_sec % atoi(it->parameter.c_str()) == 0) {
                        std::thread workflow_thread(&WorkflowDispatcher::create_workflow,this,&(*it));
                        workflow_thread.detach();
                    }
                    break;
                }
                case 2: {
                    int seconds = -1,minutes = -1, hours = -1,days = -1,day_of_week = -1,week_of_month = -1;
                    if (it->parameter.substr(0,2) != "**")
                        seconds = atoi(it->parameter.substr(0,2).c_str());
                    if (it->parameter.substr(2,2) != "**")
                        minutes = atoi(it->parameter.substr(2,2).c_str());
                    if (it->parameter.substr(4,2) != "**")
                        hours = atoi(it->parameter.substr(4,2).c_str());
                    if (it->parameter.substr(6,2) != "**")
                        days = atoi(it->parameter.substr(6,2).c_str());
                    if (it->parameter.substr(8,2) != "**")
                        day_of_week = atoi(it->parameter.substr(8,2).c_str());
                    if (it->parameter.substr(10,2) != "**")
                        week_of_month = atoi(it->parameter.substr(10,2).c_str());
                    
                    // Check week of month
                    if (week_of_month == -1 || (time_struct->tm_mday / 7) == week_of_month){
                        // Check day of week
                        if (day_of_week == -1 ||(time_struct->tm_wday + 1) == day_of_week) {
                            // Check "every [] day"
                            if (days == -1 ||time_struct->tm_mday % days == 0) {
                                // Check "every [] hours"
                                if (hours == -1 ||time_struct->tm_hour % hours == 0) {
                                    // Check "every [] minutes"
                                    if (minutes == -1 ||time_struct->tm_min % minutes == 0) {
                                        // Check "every [] seconds"
                                        if (seconds == -1 || time_struct->tm_sec % seconds ==0) {
                                            std::thread workflow_thread(&WorkflowDispatcher::create_workflow,this,&(*it));
                                            workflow_thread.detach();
                                        }
                                    }
                                }
                            }
                        }
                    }
                    break;
                }
                default:{
                    throw std::invalid_argument("Unknown trigger type");
                    break;
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    } while (true);

    
}

void WorkflowDispatcher::create_workflow(dispatcher_entry* entry) {
    if (entry->workflow_id != 1) 
        return;
        
    std::map<unsigned int,workflow_item_struct> workflow_items;
    workflow_items.insert(std::pair<unsigned int,workflow_item_struct>(1,workflow_item_struct{"1","1",2,_block_type::collector,"CommandBlock"}));
    workflow_items.insert(std::pair<unsigned int,workflow_item_struct>(2,workflow_item_struct{"2","1",0,_block_type::condition,"SimpleEvaluationBlock"}));

    try {
        WorkflowManager* manager = new WorkflowManager("1",workflow_items);
        manager->run_workflow();

        delete manager;
    }
    catch (std::exception& e) {
        // write dispatcher errors regarding threads
    }

    
}