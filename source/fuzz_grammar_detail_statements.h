#pragma once

namespace fuzz_grammar
{
	struct AssignmentQualifier : lexy::token_production 
	{
		struct Temp : lexy::transparent_production
		{
			static constexpr auto rule = LEXY_LIT("temp");
			static constexpr auto value = lexy::constant(fuzz::AssignmentQualifier::temp);
		};
		struct Let : lexy::transparent_production
		{
			static constexpr auto rule = LEXY_LIT("let");
			static constexpr auto value = lexy::constant(fuzz::AssignmentQualifier::let);
		};

		static constexpr auto rule = dsl::p<Temp> | dsl::p<Let>;
		static constexpr auto value = lexy::callback<fuzz::AssignmentQualifier>([] (fuzz::AssignmentQualifier aq) {
			return aq;
		});
	};

	struct AssignmentStatement
	{
		static constexpr auto single_equals = dsl::not_followed_by(LEXY_LIT("="), dsl::lit_c<'='>);

		static constexpr auto rule = dsl::opt(dsl::p<AssignmentQualifier>)
			+ dsl::p<Identifier>
			+ dsl::equal_sign
			+ dsl::recurse<Expression>;

		static constexpr auto value = lexy::callback<fuzz::Statement>(
			[](std::optional<fuzz::AssignmentQualifier> qual, fuzz::Identifier id, fuzz::Expression expr )
			{
				auto result = fuzz::AssignmentStatement();
				result.qualifier = qual;
				result.name = id;
				result.expression = expr;
				return result;
			});
	};

	struct ReturnStatement {

		static constexpr auto rule = LEXY_LIT("return") + dsl::recurse<Expression>;

		static constexpr auto value = lexy::callback<fuzz::Statement>(
			[](fuzz::Expression expr)
			{
				auto result = fuzz::ReturnStatement();
				result.expression = expr;
				return result;
			});
	};

	struct Statement : lexy::token_production 
	{
		static constexpr auto whitespace = dsl::ascii::blank | dsl::newline;

		static constexpr auto rule 
			= dsl::p<ReturnStatement>
			| dsl::p<AssignmentStatement>;

		static constexpr auto value = lexy::callback<fuzz::Statement>([](auto x) { return x; });
	};
}

