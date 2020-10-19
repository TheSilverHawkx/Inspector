#include "condition_parser.h"


bool inspector::evaluate_condition(rapidjson::Document& conditions) {
    // Check for 'operator' keys
    if (conditions.HasMember("operator")) {

    }

    // iterate 'condition keys' and recurse if there are child nodes


    /*
    	1 contains "yes" AND 1 notcontains "him" OR 1 contains "maybe"

	

	contains "yes" AND (not contains "him" or contains "her")

Step 1: Break on brackets
	contains "yes" AND
	not contains "him" OR contains "her"

Step 2: Break on OR/AND

	Contains "yes"
	+
		notcontains "him"
		|
		contains "her"





1. contains "yes"
2. contains "yes" AND notcontains "yeet"
3. contains "yes" OR notcontains "yeet"
4. contains "yes" AND (notcontains "yeet" OR notcontains "feet")


1. { "condition" : "contains \"yes\"" }
2. { "operator" : "and", "condition" : "constains \"yes\"", "condition" : "notcontains \"yeet\"" }
3. { "operator" : "or", "condition" : "constains \"yes\"", "condition" : "notcontains \"yeet\"" }
4. { "operator" : "and", "condition" : "constains \"yes\"", "condition" : { "operator" : "or", "condition" : "not contains \"yeet\"", "condition" : "notcontains \"feet\""


    */
}