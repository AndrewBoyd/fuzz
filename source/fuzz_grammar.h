#pragma once
#include "fuzz_types.h"
#include <utility/string_utils.h>

#define LEXY_ENCODING_OF_CHAR lexy::utf8_encoding;
#include <lexy/action/parse.hpp> // lexy::parse
#include <lexy/callback.hpp>     // value callbacks
#include <lexy/dsl.hpp>          // lexy::dsl::*
#include <lexy/input/string_input.hpp>
#include <lexy/input/file.hpp>   // lexy::read_file
#include <lexy_ext/report_error.hpp> // lexy_ext::report_error

namespace fuzz_grammar {
	namespace dsl = lexy::dsl;
	namespace fz = fuzz;
	using string_t = fz::string_t;

	// fuzz_grammar_detail_literals.h
	struct Boolean;
	struct Number;
	struct String;
	struct Identifier;
	struct Primitive;
	
	// fuzz_grammar_detail_expressions.h
	struct Expression;
	struct PrimitiveExpression;
	struct NestedExpression;

	// fuzz_grammar_detail_statements.h
	struct AssignmentQualifier;
	struct AssignmentStatement;
	struct ReturnStatement;
	struct Statement;

}

#include "fuzz_grammar_detail_literals.h"
#include "fuzz_grammar_detail_expressions.h"
#include "fuzz_grammar_detail_statements.h"

template<typename parse_t = fuzz_grammar::Expression>
inline auto parse(std::u8string input_string)
{
	auto input = lexy::string_input<lexy::utf8_encoding>(input_string);
	auto parse_result = lexy::parse<parse_t>(input, lexy_ext::report_error);
	return parse_result.value();
}

template<typename parse_t = fuzz_grammar::Expression>
inline auto parse(std::string input_string)
{
	return parse<parse_t>(string_utils::ascii_to_u8(input_string));
}
