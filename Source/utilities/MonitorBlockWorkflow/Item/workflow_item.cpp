#include "workflow_item.h"

WorkflowItem::WorkflowItem(workflow_item_struct info,MonitorBlock* previous_monitor_block) {
    this->info = info;

    if (info.block_type == _block_type::collector) {
        // Script Bash
        //const char* json_sh = "{\"script_language\":\"bash\",\"script_parameters\":\"1 2 3\",\"script_code\":\"#!/bin/bash\\n\\nfor i in `ls`; do\\necho $i\\ndone\"}";
        //Script Powershell
        const char* json_ps = "{\"script_language\":\"powershell\",\"script_parameters\":\"1 2 3\",\"script_code\":\" Get-Content 'C:\\\\Users\\\\omerr\\\\OneDrive\\\\Desktop\\\\yeet1.txt'\"}";
        // Script Batch
        //const char* json_bat = "{\"script_language\":\"batch\",\"script_parameters\":\"1 2 3\",\"script_code\":\"echo bat1\\necho bat2\"}";
        // WMI
        const char* json_wmi = "{\"namespace\":\"root\\\\cimv2\",\"query\":\"select * from win32_service where Name like '%plugplay%'\",\"target\":\"localhost\"}";
        // Command Windows
        const char* command = "{\"commandline\":\"sc query plugplay\"}";
        // Command Linux
        //const char* command = "{\"commandline\":\"/bin/bash -c 'echo hello'\"}";
        

        if (strcmp(info.block_class,"CommandBlock")) {
            this->monitor_block = new CommandMonitorBlock("123","mock_command_block",command);
        }
        else if (info.block_class == "ScriptBlock") {
            this->monitor_block = new ScriptMonitorBlock("123","mock_script_block",json_ps);
        }
        else if (info.block_class == "WMIMonitorBlock") {
            this->monitor_block = new WMIMonitorBlock("123","mock_WMI_block",json_wmi);
        }
        else {
            throw std::runtime_error("Error creating Monitorblock. Unknown block_type.");
        }


    }
    else if (info.block_type == _block_type::condition) {
        // Mock data for Conditions
        // Simple Condition
        //const char* mock_condition = "{\"condition_operator\":\"contains\",\"condition_value\": \"Video\",\"index\" : 0}";
        // AND Condition
        //const char* mock_condition = "{\"group_operator\" : \"and\", \"conditions\": [{\"condition_operator\":\"contains\",\"condition_value\": \"Video\",\"index\" : 0},{\"condition_operator\":\"contains\",\"condition_value\": \"due\",\"index\" : 0}]}";
        // OR Condition
        const char* mock_condition = "{\"group_operator\" : \"or\", \"conditions\": [{\"condition_operator\":\"contains\",\"condition_value\": \"video\",\"index\" : 0},{\"condition_operator\":\"contains\",\"condition_value\": \"Video\",\"index\" : 0}]}";
        // NESTED Condition
        //const char* mock_condition = "{\"group_operator\" : \"and\", \"conditions\": [{\"condition_operator\":\"equals\",\"condition_value\": \"4\",\"index\" : 0},{\"group_operator\" : \"or\", \"conditions\": [{\"condition_operator\":\"equals\",\"condition_value\": \"5\",\"index\" : 0},{\"condition_operator\":\"equals\",\"condition_value\": \"6\",\"index\" : 0}]}]}";

            
        if (info.block_class == "SimpleEvaluationBlock") {
            this->monitor_block = new SimpleEvaluationBlock("123",mock_condition,(CollectorMonitorBlock*)previous_monitor_block);
        }
        else {
            throw std::runtime_error("Error creating Monitorblock. Unknown block_type.");
        }
    }
    else {
        throw std::invalid_argument("Unknown block_type.");
    }

}

WorkflowItem::~WorkflowItem() {
    delete this->monitor_block;
};

bool WorkflowItem::run_item(){
    try {
        return this->monitor_block->run_block();
    }
    catch (std::exception& e){
        throw;
    }
}