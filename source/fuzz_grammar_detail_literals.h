#pragma once

namespace fuzz_grammar
{
	struct Boolean : lexy::token_production {
		struct true_ : lexy::transparent_production {
			static constexpr auto rule = LEXY_LIT(u8"true");
			static constexpr auto value = lexy::constant(true);
		};
		struct false_ : lexy::transparent_production {
			static constexpr auto rule = LEXY_LIT(u8"false");
			static constexpr auto value = lexy::constant(false);
		};
		static constexpr auto rule = dsl::p<true_> | dsl::p<false_>;
		static constexpr auto value = lexy::construct<fz::Boolean>;
	};

	struct Number : lexy::token_production {
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
			= lexy::callback<double>([] (std::int64_t num, std::optional<fz::string_t> frac)
			{
				auto result = static_cast<double>(num);
				if (frac.has_value()) {
					auto as_string = string_utils::u8_to_ascii(*frac);
					auto as_float = std::stod("0." + as_string);
					result += as_float;
				}
				return result;
			});
	};

	struct String : lexy::token_production
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

		static constexpr auto value = lexy::as_string<fz::String, lexy::utf8_encoding>;
	};

	// TODO: Identifiers need to use the unicode database.
	struct Identifier
	{
		static constexpr auto rule
			= dsl::identifier(dsl::ascii::alpha, dsl::ascii::alnum);
		static constexpr auto value 
			= lexy::as_string<fz::String, lexy::utf8_encoding> 
			| lexy::construct<fz::Identifier>;
	};

	struct Primitive : lexy::transparent_production {
		static constexpr auto rule
			= dsl::p<Boolean>
			| dsl::p<Number>
			| dsl::p<String>
			| dsl::p<Identifier>;

		static constexpr auto value = lexy::forward<fz::Primitive>;
	};
}

