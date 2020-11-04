#include "condition_parser.h"


bool inspector::evaluate_condition(rapidjson::Document& conditions_list, std::vector<std::string>& output_list) {
	bool condition_status {false};
	std::string group_operator {""};

    // Check for Condition with "AND"/"OR" by looking for 'operator' keys
    if (conditions_list.HasMember("group_operator")) {
		std::string group_operator = conditions_list["group_operator"].GetString();
	}

	// Iterate conditions in condition list at the same level
	for (auto& condition : conditions_list["field"].GetArray())
	{
		// If condition has a memmber name "group_operator" -> there is a nested condition. recurse function.
		if (condition.HasMember("group_operator")) {
			condition_status = inspector::evaluate_condition(condition,output_list);
		}
		else {
			// Seperate condition into operator and subject. Example:
			// "contains \"foo\""
			//	^^^^^^^^   ^^^
			// operator	  subject
			const std::string& rule = condition.GetString();
			const char* condition_operator = rule.substr(0,rule.find(" ")).c_str();
			std::string condition_subject = rule.substr(rule.find("\""),rule.length()-2);

			condition_status = inspector::operator_table[condition_operator](output_list,condition_subject);
		}

		// Shortcut AND / OR groups on first instance of status:
		// OR = at least 1 true to be good.
		// AND = at least 1 false  to be bad.
		// Irrelevant for single-condition lists.
		if (group_operator == "and" && condition_status == false) return false;

		if (group_operator == "or" && condition_status == true) return true;

	}

	// Iterated through all the conditions and didn't exit on shortcut, therefore at this point:
	// AND = passed all conditions with true
	// OR = passed all conditions with false
	// NONE = single condition, either true / false
	return condition_status;
}


// Function of recursion only, uses rapidjson::Value instaed of rapidjson::Document
bool inspector::evaluate_condition(rapidjson::Value& conditions_list, std::vector<std::string>& output_list) {

    bool condition_status {false};
	std::string group_operator {""};

    // Check for Condition with "AND"/"OR" by looking for 'operator' keys
    if (conditions_list.HasMember("group_operator")) {
		std::string group_operator = conditions_list["group_operator"].GetString();
	}

	// Iterate conditions in condition list at the same level
	for (auto& condition : conditions_list["field"].GetArray())
	{
		// If condition has a memmber name "group_operator" -> there is a nested condition. recurse function.
		if (condition.HasMember("group_operator")) {
			condition_status = inspector::evaluate_condition(condition,output_list);
		}
		else {
			// Seperate condition into operator and subject. Example:
			// "contains \"foo\""
			//	^^^^^^^^   ^^^
			// operator	  subject
			const std::string& rule = condition.GetString();
			const char* condition_operator = rule.substr(0,rule.find(" ")).c_str();
			std::string condition_subject = rule.substr(rule.find("\""),rule.length()-2);
			condition_status = operator_table[condition_operator](output_list,condition_subject);
		}

		// Shortcut AND / OR groups on first instance of status:
		// OR = at least 1 true to be good.
		// AND = at least 1 false  to be bad.
		// Irrelevant for single-condition lists.
		if (group_operator == "and" && condition_status == false) return false;

		if (group_operator == "or" && condition_status == true) return true;

	}

	// Iterated through all the conditions and didn't exit on shortcut, therefore at this point:
	// AND = passed all conditions with true
	// OR = passed all conditions with false
	// NONE = single condition, either true / false
	return condition_status;
}
