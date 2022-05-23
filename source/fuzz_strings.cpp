#include "fuzz_strings.h"
#include <utility/string_utils.h>
#include <fmt/format.h>

namespace fuzz
{
	static auto const kToString_fuzz_binary_operator =
		std::map<BinaryOperator, std::string>
	{
		{ BinaryOperator::add, "add" },
		{ BinaryOperator::subtract, "subtract" },
		{ BinaryOperator::multiply, "multiply" },
		{ BinaryOperator::divide, "divide" },
		{ BinaryOperator::modulo, "modulo" },
		{ BinaryOperator::less_than, "less_than" },
		{ BinaryOperator::less_than_or_equal, "less_than_or_equal" },
		{ BinaryOperator::greater_than, "greater_than" },
		{ BinaryOperator::greater_than_or_equal, "greater_than_or_equal" },
		{ BinaryOperator::equal, "equal" },
		{ BinaryOperator::not_equal, "not_equal" },
		{ BinaryOperator::logical_and, "logical_and" },
		{ BinaryOperator::logical_or, "logical_or" },
	};

	static auto const kBinaryOperatorShortForm =
		std::map<BinaryOperator, std::string>
	{
		{ BinaryOperator::add, "+" },
		{ BinaryOperator::subtract, "-" },
		{ BinaryOperator::multiply, "*" },
		{ BinaryOperator::divide, "/" },
		{ BinaryOperator::modulo, "%" },
		{ BinaryOperator::less_than, "<" },
		{ BinaryOperator::less_than_or_equal, "<=" },
		{ BinaryOperator::greater_than, ">" },
		{ BinaryOperator::greater_than_or_equal, ">=" },
		{ BinaryOperator::equal, "==" },
		{ BinaryOperator::not_equal, "!=" },
		{ BinaryOperator::logical_and, "&&" },
		{ BinaryOperator::logical_or, "||" },
	};

	static auto const kToString_fuzz_assignment_qualifier =
		std::map<AssignmentQualifier, std::string>
	{
		{ AssignmentQualifier::let, "let" },
		{ AssignmentQualifier::temp, "temp" },
	};

	std::string to_string(Boolean x) {
		return fmt::format("boolean: {}", x);
	}

	std::string to_string(Number x) {
		return fmt::format("number: {}", x);
	}

	std::string to_string(String x) {
		return fmt::format("string: \"{}\"", string_utils::u8_to_ascii(x));
	}

	std::string to_string(Identifier x) {
		return fmt::format("Identifier: \"{}\"", string_utils::u8_to_ascii(x.id));
	}

	std::string to_string(Primitive x) {
		return std::visit([](auto value) {
			return to_string(value);
		}, x);
	}

	std::string to_string(Array)
	{
		return std::string();
	}

	std::string to_string(Block)
	{
		return std::string();
	}

	std::string to_string(Lambda)
	{
		return std::string();
	}

	std::string to_string(Expression x)
	{
		auto expr_ptr = x.get();
		if (auto expr = dynamic_cast<PrimitiveExpression*>(expr_ptr))
			return to_string(*expr);
		if (auto expr = dynamic_cast<BinaryOperation*>(expr_ptr))
			return to_string(*expr);

		return "Unknown Expression";
	}

	std::string to_string(PrimitiveExpression x)
	{
		return fmt::format("<{}>", to_string(x.primitive));
	}

	std::string to_string(BinaryOperation x)
	{
		return fmt::format("({1} {0} {2})",
			kBinaryOperatorShortForm.at(x.operation),
			to_string(x.lhs),
			to_string(x.rhs));
	}

	std::string to_string(AssignmentQualifier x)
	{
		return kToString_fuzz_assignment_qualifier.at(x);
	}

	std::string to_string(Statement x)
	{
		auto statement_ptr = x.get();
		if (auto statement = dynamic_cast<AssignmentStatement*>(statement_ptr))
			return to_string(*statement);
		if (auto statement = dynamic_cast<ReturnStatement*>(statement_ptr))
			return to_string(*statement);

		return "Unknown Statement";
	}

	std::string to_string(AssignmentStatement x)
	{
		if (x.qualifier) 
		{
			return fmt::format("{} {} = {};",
				to_string(*x.qualifier),
				to_string(x.name),
				to_string(x.expression));
		}

		return fmt::format("{} = {};",
			to_string(x.name),
			to_string(x.expression));
	}

	std::string to_string(ReturnStatement x)
	{
		return fmt::format("return {};",
			to_string(x.expression));
	}

	//std::string to_string(fuzz_array x) {
	//	auto strings = std::vector<std::string>{ };
	//	strings.reserve(x.values.size());
	//	for (auto v : x.values) {
	//		strings.push_back(to_string(v));
	//	}
	//	auto concat = string_utils::concat(strings, ", ");
	//	return fmt::format("array[{}]: [{}]", x.values.size(), concat);
	//}

}
