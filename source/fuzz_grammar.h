#pragma once
#include "fuzz_types.h"

#define LEXY_ENCODING_OF_CHAR lexy::utf8_encoding;
#include <lexy/action/parse.hpp> // lexy::parse
#include <lexy/callback.hpp>     // value callbacks
#include <lexy/dsl.hpp>          // lexy::dsl::*
#include <lexy/input/string_input.hpp>
#include <lexy/input/file.hpp>   // lexy::read_file
#include <lexy_ext/report_error.hpp> // lexy_ext::report_error

#define FUZZ_GRAMMAR_BINARY_OPERATOR( name, op )                                            \
struct name : lexy::transparent_production {                                                \
	static constexpr auto rule = LEXY_LIT(op);                                              \
	static constexpr auto value = lexy::constant(fz::fuzz_binary_operator:: ## name);       \
};                                                                                          //

namespace fuzz_grammar
{
	namespace dsl = lexy::dsl;
	namespace fz = fuzz;
	using string_t = fz::string_t;
	struct f_value;
	struct binary_operation;
	struct expression;

	struct boolean : lexy::token_production {
		struct true_ : lexy::transparent_production {
			static constexpr auto rule = LEXY_LIT(u8"true");
			static constexpr auto value = lexy::constant(true);
		};
		struct false_ : lexy::transparent_production {
			static constexpr auto rule = LEXY_LIT(u8"false");
			static constexpr auto value = lexy::constant(false);
		};
		static constexpr auto rule = dsl::p<true_> | dsl::p<false_>;
		static constexpr auto value = lexy::construct<fz::fuzz_boolean>;
	};

	struct number : lexy::token_production {
		struct integer : lexy::transparent_production
		{
			static constexpr auto rule
				= dsl::minus_sign + dsl::integer<std::int64_t>(dsl::digits<>.no_leading_zero());
			static constexpr auto value
				= lexy::as_integer<std::int64_t>;
		};

		struct fraction : lexy::transparent_production
		{
			static constexpr auto rule
				= dsl::lit_c<'.'> >> dsl::capture(dsl::digits<>);
			static constexpr auto value
				= lexy::as_string<fz::string_t>;
		};

		static constexpr auto rule
			= dsl::peek(dsl::lit_c<'-'> / dsl::digit<>)
			>> dsl::p<integer> +dsl::opt(dsl::p<fraction>);
		static constexpr auto value
			= lexy::construct< fz::fuzz_number >;
	};

	struct string_basic : lexy::token_production
	{
		struct invalid_char
		{
			static LEXY_CONSTEVAL auto name()
			{
				return "invalid character in string literal";
			}
		};

		// A mapping of the simple escape sequences to their replacement values.
		static constexpr auto escaped_symbols = lexy::symbol_table<char> //
			.map<'"'>('"')
			.map<'\\'>('\\')
			.map<'/'>('/')
			.map<'b'>('\b')
			.map<'f'>('\f')
			.map<'n'>('\n')
			.map<'r'>('\r')
			.map<'t'>('\t');

		static constexpr auto rule = [] {
			// Everything is allowed inside a string except for control characters.
			auto code_point = (-dsl::unicode::control).error<invalid_char>;

			// Escape sequences start with a backlash and either map one of the symbols,
			// or a Unicode code point of the form uXXXX.
			auto escape = dsl::backslash_escape //
				.symbol<escaped_symbols>()
				.rule(dsl::lit_c<'u'> >> dsl::code_point_id<4>);

			// String of code_point with specified escape sequences, surrounded by ".
			// We abort string parsing if we see a newline to handle missing closing ".
			return dsl::quoted.limit(dsl::ascii::newline)(code_point, escape);
		}();

		static constexpr auto value = lexy::as_string<string_t, lexy::utf8_encoding>;
	};

	struct string : lexy::token_production {
		static constexpr auto rule = dsl::p<string_basic>;
		static constexpr auto value = lexy::construct<fz::fuzz_string>;
	};

	struct array_basic
	{
		static constexpr auto rule
			= dsl::square_bracketed
			.opt_list(dsl::recurse<expression>, dsl::trailing_sep(dsl::comma));

		static constexpr auto value = lexy::as_list<std::vector<fz::fuzz_expression>>;
	};

	struct array
	{
		static constexpr auto rule = dsl::p<array_basic>;
		static constexpr auto value = lexy::callback<fz::fuzz_array>([](std::vector<fz::fuzz_expression> basic)
		{
			auto result = fz::fuzz_array{};
			for (auto value : basic) {
				result.values.emplace_back(std::move(value));
			}
			return result;
		});
	};

	struct f_value : lexy::token_production
	{
		static constexpr auto name = "fuzz value";

		struct expected_fuzz_value {
			static LEXY_CONSTEVAL auto name() {
				return "expected fuzz value";
			}
		};

		static constexpr auto rule 
			= dsl::p<boolean> 
			| dsl::p<number> 
			| dsl::p<string> 
			| dsl::p<array>;
		static constexpr auto value = lexy::callback<fz::fuzz_value>([](auto&& x) {
			return fz::makeFuzzObject(x);
		});
	};

	struct binary_operator : lexy::transparent_production
	{
		FUZZ_GRAMMAR_BINARY_OPERATOR(add, "+");
		FUZZ_GRAMMAR_BINARY_OPERATOR(subtract, "-");
		FUZZ_GRAMMAR_BINARY_OPERATOR(multiply, "*");
		FUZZ_GRAMMAR_BINARY_OPERATOR(divide, "/");
		FUZZ_GRAMMAR_BINARY_OPERATOR(modulo, "%");
		FUZZ_GRAMMAR_BINARY_OPERATOR(less_than, "<");
		FUZZ_GRAMMAR_BINARY_OPERATOR(less_than_or_equal, "<=");
		FUZZ_GRAMMAR_BINARY_OPERATOR(greater_than, ">");
		FUZZ_GRAMMAR_BINARY_OPERATOR(greater_than_or_equal, ">=");
		FUZZ_GRAMMAR_BINARY_OPERATOR(equal, "==");
		FUZZ_GRAMMAR_BINARY_OPERATOR(not_equal, "!=");
		FUZZ_GRAMMAR_BINARY_OPERATOR(logical_and, "&&");
		FUZZ_GRAMMAR_BINARY_OPERATOR(logical_or, "||");

		static constexpr auto rule = dsl::p<add> 
		                           | dsl::p<subtract>
			                       | dsl::p<multiply>
			                       | dsl::p<divide>
			                       | dsl::p<modulo>
			                       | dsl::p<less_than_or_equal>
			                       | dsl::p<greater_than_or_equal>
			                       | dsl::p<less_than>
			                       | dsl::p<greater_than>
			                       | dsl::p<equal>
			                       | dsl::p<not_equal>
			                       | dsl::p<logical_and>
			                       | dsl::p<logical_or>
			                       | dsl::p<multiply>;
		static constexpr auto value = lexy::construct<fz::fuzz_binary_operator>;

	};

	struct binary_operation : lexy::token_production
	{
		static constexpr auto rule
			= dsl::recurse<expression>
			+ dsl::p<binary_operator>
			+ dsl::recurse<expression>;
		static constexpr auto value
			= lexy::construct<fz::fuzz_binary_operation>;
	};

	struct bracketed_expression : lexy::transparent_production 
	{
		static constexpr auto rule = dsl::parenthesized(dsl::recurse<expression>);
		static constexpr auto value = lexy::construct<fz::fuzz_expression>;
	};

	struct expression : lexy::token_production
	{
		struct evaluated_expression : lexy::transparent_production {
			static constexpr auto rule
				= dsl::recurse<bracketed_expression>
				| dsl::recurse<binary_operation>;
			static constexpr auto value 
				= lexy::construct<fz::fuzz_expression>;
		};

		static constexpr auto whitespace = dsl::ascii::space / dsl::ascii::newline;
		static constexpr auto rule 
			= dsl::p<evaluated_expression>
			| dsl::p<f_value>;

		static constexpr auto value = lexy::constant<fz::fuzz_expression>;
	};
}
