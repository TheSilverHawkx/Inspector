#include "workflow_item.h"

WorkflowItem::WorkflowItem(workflow_item_struct info,WorkflowItem* previous_workflow_item) {
    this->info = info;

    if (info.block_type == _block_type::collector) {
        /*
        // Script Bash
        //const char* json_sh = "{\"script_language\":\"bash\",\"script_parameters\":\"1 2 3\",\"script_code\":\"#!/bin/bash\\n\\nfor i in `ls`; do\\necho $i\\ndone\"}";
        //Script Powershell
        //const char* json_ps = "{\"script_language\":\"powershell\",\"script_parameters\":\"1 2 3\",\"script_code\":\" Get-Content 'C:\\\\Users\\\\omerr\\\\OneDrive\\\\Desktop\\\\yeet1.txt'\"}";
        // Script Batch
        //const char* json_bat = "{\"script_language\":\"batch\",\"script_parameters\":\"1 2 3\",\"script_code\":\"echo bat1\\necho bat2\"}";
        // WMI
        //const char* json_wmi = "{\"namespace\":\"root\\\\cimv2\",\"query\":\"select * from win32_service where Name like '%plugplay%'\",\"target\":\"localhost\"}";
        // Command Windows
        //const char* command = "{\"commandline\":\"sc query plugplay\"}";
        // Command Linux
        //const char* command = "{\"commandline\":\"/bin/bash -c 'echo hello'\"}";*/
        

        if (info.block_class.compare("CommandMonitorBlock") == 0) {
            this->monitor_block = new CommandMonitorBlock(info.block_id.c_str(),"mock_command_block",info.parameters.c_str());
        }
        else if (info.block_class.compare("ScriptMonitorBlock") == 0) {
            this->monitor_block = new ScriptMonitorBlock(info.block_id.c_str(),"mock_script_block",info.parameters.c_str());
        }
        else if (info.block_class.compare("WMIMonitorBlock") == 0) {
            this->monitor_block = new WMIMonitorBlock(info.block_id.c_str(),"mock_WMI_block",info.parameters.c_str());
        }
        else {
            throw inspector::MonitorBlockException(info.block_id.c_str(),"Error creating Monitorblock. Unknown block class found.");
        }
    }
    else if (info.block_type == _block_type::condition) {
        /*
        // Mock data for Conditions
        // Simple Condition
        //const char* mock_condition = "{\"condition_operator\":\"contains\",\"condition_value\": \"Video\",\"index\" : 0}";
        // AND Condition
        //const char* mock_condition = "{\"group_operator\" : \"and\", \"conditions\": [{\"condition_operator\":\"contains\",\"condition_value\": \"Video\",\"index\" : 0},{\"condition_operator\":\"contains\",\"condition_value\": \"due\",\"index\" : 0}]}";
        // OR Condition
        //const char* mock_condition = "{\"group_operator\" : \"or\", \"conditions\": [{\"condition_operator\":\"contains\",\"condition_value\": \"video\",\"index\" : 0},{\"condition_operator\":\"contains\",\"condition_value\": \"Video\",\"index\" : 0}]}";
        // NESTED Condition
        //const char* mock_condition = "{\"group_operator\" : \"and\", \"conditions\": [{\"condition_operator\":\"equals\",\"condition_value\": \"4\",\"index\" : 0},{\"group_operator\" : \"or\", \"conditions\": [{\"condition_operator\":\"equals\",\"condition_value\": \"5\",\"index\" : 0},{\"condition_operator\":\"equals\",\"condition_value\": \"6\",\"index\" : 0}]}]}";
        */
            
        if (info.block_class == "SimpleEvaluationBlock") {
            this->monitor_block = new SimpleEvaluationBlock(info.block_id.c_str(),info.parameters.c_str(),(CollectorMonitorBlock*)previous_workflow_item->monitor_block);
        }
        else {
            throw inspector::MonitorBlockException(info.block_id.c_str(),"Error creating Monitorblock. Unknown block class found.");
        }
    }
    else {
        throw inspector::MonitorBlockException(info.block_id.c_str(),"Error creating Monitorblock. Unknown block type found.");
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