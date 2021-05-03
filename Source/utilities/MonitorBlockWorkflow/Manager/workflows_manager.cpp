#include "workflows_manager.h"

WorkflowManager::WorkflowManager(const char* id,std::map<unsigned int,workflow_item_struct>& items) {
    this->workflow_id = id;
    this->workflow_items = items;
};

WorkflowManager::~WorkflowManager(){};

void WorkflowManager::run_workflow() {
    WorkflowItem* current_item = nullptr;
    WorkflowItem* previous_item = nullptr;

    try {
        for (workflow_item_struct* is = &(this->workflow_items[1]); is->id != 0; is = &(this->workflow_items[is->next_id])){
            current_item = new WorkflowItem(*is,previous_item);
            delete previous_item;

            if (!current_item->run_item()) {
                std::string err_msg {"Workflow Item with id '" };
                err_msg.append(std::to_string(is->id));
                err_msg.append("' did not finish successfully.");

                throw inspector::WorkflowException(this->workflow_id,err_msg.c_str());
            }

            previous_item = current_item;

            // Handle write to log about workflow finished.
        }
        delete current_item;
    }
    catch (std::exception& e){
        std::string kaki= e.what();
        delete previous_item;
        delete current_item;
    }
}
