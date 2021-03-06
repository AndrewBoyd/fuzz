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

	static auto const kToString_fuzz_keyword_operator =
		std::map<PrefixKeyword, std::string>
	{
		{ PrefixKeyword::return_, "return" },
		{ PrefixKeyword::let, "let" },
		{ PrefixKeyword::temp, "temp" },
	};

	std::string to_string(Boolean x) {
		return fmt::format("bool: {}", x);
	}

	std::string to_string(Number x) {
		return fmt::format("#: {}", x);
	}

	std::string to_string(String x) {
		return fmt::format("str: \"{}\"", string_utils::u8_to_ascii(x));
	}

	std::string to_string(Identifier x) {
		return fmt::format("id: \"{}\"", string_utils::u8_to_ascii(x.id));
	}

	std::string to_string(Primitive x) {
		return std::visit([](auto value) {
			return to_string(value);
		}, x);
	}

	std::string to_string(Array x)
	{
		auto strings = std::vector<std::string>{ };
		strings.reserve(x.size());
		for (auto v : x) {
			strings.push_back(to_string(v));
		}
		auto concat = string_utils::concat(strings, ", ");
		return fmt::format("array[{}]: [{}]", x.size(), concat);
	}

	std::string to_string(Block x)
	{
		auto & statements = x.statements;
		auto strings = std::vector<std::string>{ };
		strings.reserve(statements.size());
		for (auto statement : statements) {
			strings.push_back(to_string(statement));
		}
		auto concat = string_utils::concat(strings, "; ");
		return fmt::format("block: {{ {} }}", concat);
	}

	std::string to_string(Lambda x)
	{
		auto& statements = x.block.statements;
		auto strings = std::vector<std::string>{ };
		strings.reserve(statements.size());
		for (auto statement : statements) {
			strings.push_back(to_string(statement));
		}
		auto concat = string_utils::concat(strings, "; ");

		auto fv_strings = std::vector<std::string>{ "" };
		for (auto fv : x.free_parameters) {
			fv_strings.push_back(
				string_utils::to_ascii(fv.id)
			);
		}
		auto free_variables = string_utils::concat(fv_strings, "/");

		return fmt::format("{}->{{ {} }}", free_variables, concat);
	}

	std::string to_string(Object x)
	{
		auto& values = x.values;
		auto strings = std::vector<std::string>{ };
		strings.reserve(values.size());
		for (auto &[id, value] : values) {
			strings.push_back(string_utils::to_ascii(id));
		}
		auto concat = string_utils::concat(strings, ", ");
		return fmt::format("Object: {{ {} }}", concat);
	}

	std::string to_string(Expression x)
	{
		auto expr_ptr = x.get();
		if (auto expr = dynamic_cast<PrimitiveExpression*>(expr_ptr))
			return to_string(*expr);
		if (auto expr = dynamic_cast<BinaryOperation*>(expr_ptr))
			return to_string(*expr);
		if (auto expr = dynamic_cast<KeywordOperation*>(expr_ptr))
			return to_string(*expr);
		if (auto expr = dynamic_cast<AssignOperation*>(expr_ptr))
			return to_string(*expr);
		if (auto expr = dynamic_cast<Evaluation*>(expr_ptr))
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
			kBinaryOperatorShortForm.at(x.operation_),
			to_string(x.lhs_),
			to_string(x.rhs_));
	}

	std::string to_string(Evaluation x)
	{
		return fmt::format("({}.{})",
				to_string(x.to_evaluate),
				to_string(x.parameter_pack));;
	}

	std::string to_string(KeywordOperation x)
	{
		return fmt::format("{} {}", 
			kToString_fuzz_keyword_operator.at(x.keyword),
			to_string(x.expr));
	}

	std::string to_string(AssignOperation x)
	{
		return fmt::format("&{} := {}",
			to_string(x.lhs),
			to_string(x.rhs));
	}


}
