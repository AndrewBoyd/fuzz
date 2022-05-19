#include "fuzz_strings.h"
#include <utility/string_utils.h>
#include <fmt/format.h>

namespace fuzz
{
	//static auto const kToString_fuzz_binary_operator =
	//	std::map<fuzz_binary_operator, std::string>
	//{
	//	{ fuzz_binary_operator::add, "add" },
	//	{ fuzz_binary_operator::subtract, "subtract" },
	//	{ fuzz_binary_operator::multiply, "multiply" },
	//	{ fuzz_binary_operator::divide, "divide" },
	//	{ fuzz_binary_operator::modulo, "modulo" },
	//	{ fuzz_binary_operator::less_than, "less_than" },
	//	{ fuzz_binary_operator::less_than_or_equal, "less_than_or_equal" },
	//	{ fuzz_binary_operator::greater_than, "greater_than" },
	//	{ fuzz_binary_operator::greater_than_or_equal, "greater_than_or_equal" },
	//	{ fuzz_binary_operator::equal, "equal" },
	//	{ fuzz_binary_operator::not_equal, "not_equal" },
	//	{ fuzz_binary_operator::logical_and, "logical_and" },
	//	{ fuzz_binary_operator::logical_or, "logical_or" },
	//};

	//std::string to_string(fuzz_boolean x) {
	//	return fmt::format("boolean: {}", x.value);
	//}

	//std::string to_string(fuzz_number x) {
	//	auto to_number = [](fuzz_number num) {
	//		auto result = static_cast<double>(num.integer);
	//		if (num.fraction.has_value()) {
	//			auto as_string = string_utils::u8string_to_string(*num.fraction);
	//			auto as_float = std::stod("0." + as_string);
	//			result += as_float;
	//		}
	//		return result;
	//	};

	//	return fmt::format("number: {}", to_number(x));
	//}

	//std::string to_string(fuzz_string x) {
	//	return fmt::format("string: \"{}\"", string_utils::u8string_to_string(x.value));
	//}

	//std::string to_string(fuzz_array x) {
	//	auto strings = std::vector<std::string>{ };
	//	strings.reserve(x.values.size());
	//	for (auto v : x.values) {
	//		strings.push_back(to_string(v));
	//	}
	//	auto concat = string_utils::concat(strings, ", ");
	//	return fmt::format("array[{}]: [{}]", x.values.size(), concat);
	//}

	//std::string to_string(fuzz_value x) {
	//	auto value_string = std::visit([](auto value) {
	//		return to_string(value);
	//		}, x->value);
	//	return fmt::format("<{}>", value_string);
	//}

	//std::string to_string(fuzz_binary_operator x)
	//{
	//	return fmt::format("|binop: {}|", kToString_fuzz_binary_operator.at(x));
	//}

	//std::string to_string(fuzz_binary_operation x)
	//{
	//	return fmt::format("({1} {0} {2})",
	//		to_string(x.lhs),
	//		kToString_fuzz_binary_operator.at(x.operation),
	//		to_string(x.rhs));
	//}

	//std::string to_string(fuzz_expression x)
	//{
	//	return std::visit([](auto value) {
	//		return to_string(value);
	//	}, x->value);
	//}

}
