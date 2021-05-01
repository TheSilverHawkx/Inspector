#include "workflows_manager.h"
#include <iostream>

WorkflowManager::WorkflowManager(const char* id,std::map<unsigned int,workflow_item_struct>& items) {
    this->workflow_id = id;
    this->workflow_items = items;
};

WorkflowManager::~WorkflowManager(){};

void WorkflowManager::run_workflow() {

    WorkflowItem* current_item = nullptr;
    WorkflowItem* previous_item = nullptr;

    try {
        for (workflow_item_struct is = this->workflow_items[1]; is.id <= this->workflow_items.size(); is = this->workflow_items[is.next_id]){
            current_item = new WorkflowItem(is);
            delete previous_item;

            if (!current_item->run_item()) {
                std::string err_msg {"Workflow Item with id '" };
                err_msg.append(std::to_string(is.id));
                err_msg.append("' did not finish successfully.");

                throw inspector::WorkflowException(this->workflow_id,err_msg.c_str());
            }

            previous_item = current_item;
        }
    }
    catch (std::exception& e){
        std::string kaki= e.what();
    }
    /*try {
        workflow_item_struct* current_workflows_item = &this->workflow_items[this->workflow_items[1].next_id];
        
        while (current_workflows_item)
        {
            ///
            /// Pull data from DB
            ///

            //workflow_item = new WorkflowItem(current_workflows_item->next_id,current_workflows_item->block_id,this->workflow_id,current_workflows_item->mock_block_type,current_workflows_item->mock_block_class,previous_item->monitor_block);
            delete previous_item;

            if (!workflow_item->run_item()){
                throw "Workflow item (id " + std::string(this->workflow_items[1].id) + ") failed.";
            }

            previous_item = workflow_item;

            if (current_workflows_item->next_id != 0) {
                current_workflows_item = &this->workflow_items[current_workflows_item->next_id];
            }
            else {
                current_workflows_item = nullptr;
            }
        }

        std::cout << "Workflow (id " + std::string(this->workflow_id) + ") has successfully finished.";
        
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        delete workflow_item;
        delete previous_item;
    }*/
}
