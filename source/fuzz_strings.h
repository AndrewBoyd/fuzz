#pragma once
#include "fuzz_types.h"
#include <string>

namespace fuzz
{
	std::string to_string(fuzz_boolean);
	std::string to_string(fuzz_number); 
	std::string to_string(fuzz_string); 
	std::string to_string(fuzz_array);
	std::string to_string(fuzz_value); 
	std::string to_string(fuzz_binary_operator);
	std::string to_string(fuzz_binary_operation);
	std::string to_string(fuzz_expression);
}
