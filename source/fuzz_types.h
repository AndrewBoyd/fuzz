#pragma once
#include <utility/value_ptr.h>
#include <optional>
#include <variant>
#include <vector>
#include <string>
#include <map>

namespace fuzz
{
	using string_t = std::u8string;

	struct fuzz_value_base;
	using fuzz_value = stdish::value_ptr<fuzz_value_base>;

	struct fuzz_block_base;
	using fuzz_block = stdish::value_ptr<fuzz_block_base>;

	struct fuzz_expression_base;
	using fuzz_expression = stdish::value_ptr<fuzz_expression_base>;

	enum class fuzz_binary_operator {
		add, subtract, multiply, divide, modulo,

		less_than, less_than_or_equal, greater_than, greater_than_or_equal, equal, not_equal,

		logical_and, logical_or
	};
;

	enum class fuzz_assignment_qualifier {
		null, let, require
	};

	enum class fuzz_statement_type {
		assignment, return_value, execute_statement
	};

	enum class fuzz_value_type {
		boolean, number, string, array, block
	};

	struct fuzz_boolean {
		bool value;
	};

	struct fuzz_number {
		std::int64_t integer;
		std::optional<string_t> fraction;
	};

	struct fuzz_string {
		string_t value;
	};

	struct fuzz_array {
		std::vector<fuzz_expression> values;
	};

	struct fuzz_name {
		std::string name;
	};
	
	struct fuzz_binary_operation {
		fuzz_expression lhs;
		fuzz_binary_operator operation;
		fuzz_expression rhs;
	};

	struct fuzz_assignment {
		fuzz_assignment_qualifier qualifier;
		string_t assignee;
		std::optional<fuzz_expression> assigned_value;
	};

	struct fuzz_return_value {
		fuzz_expression return_value;
	};

	struct fuzz_execute_statement {
		fuzz_expression to_execute;
	};

	struct fuzz_statement 
	{
		std::variant<
			fuzz_assignment,
			fuzz_return_value,
			fuzz_execute_statement
		> statement;
	};

	struct fuzz_block_base {
		std::vector< fuzz_statement > statements;
	};

	struct fuzz_value_base {
		using fuzz_value = std::variant< 
			fuzz_boolean, 
			fuzz_number, 
			fuzz_string, 
			fuzz_array
		> ;//, fuzz_block >;

		fuzz_value value;
	};

	struct fuzz_expression_base {
		std::variant< 
			fuzz_binary_operator, 
			fuzz_value
		> value;
	};


	fuzz_value makeFuzzValue(auto input) {
		auto base = fuzz_value_base{
			.value = std::move(input) 
		};

		return fuzz_value(base);
	};

	fuzz_value makeFuzzExpression(auto input) {
		auto base = fuzz_expression_base{
			.value = std::move(input)
		};

		return fuzz_expressiont(base);
	};
}
