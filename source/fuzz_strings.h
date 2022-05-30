#pragma once
#include "fuzz_types.h"
#include <string>

namespace fuzz
{
	std::string to_string(Boolean);
	std::string to_string(Number); 
	std::string to_string(String); 
	std::string to_string(Identifier); 
	std::string to_string(Primitive); 
	std::string to_string(Array);
	std::string to_string(Block);
	std::string to_string(Lambda);
	std::string to_string(Object);

	std::string to_string(Expression);
	std::string to_string(PrimitiveExpression); 
	std::string to_string(BinaryOperation); 
	std::string to_string(Evaluation); 

	std::string to_string(KeywordOperation);
	std::string to_string(AssignOperation);
}
