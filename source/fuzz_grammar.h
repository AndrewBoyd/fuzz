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
	//struct ProductExpression;
	//struct SumExpression;
	//struct ComparisonExpression;
}

#include "fuzz_grammar_detail_literals.h"
#include "fuzz_grammar_detail_expressions.h"
