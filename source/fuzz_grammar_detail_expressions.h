#pragma once

namespace fuzz_grammar
{
	struct PrimitiveExpression : lexy::transparent_production
	{
		static constexpr auto rule = dsl::p<Primitive>;
		static constexpr auto value = lexy::callback<fz::Expression>([](auto p)
		{
			auto result = fz::PrimitiveExpression{};
			result.primitive = std::move(p);
			return result;
		});
	};

	struct NestedExpression : lexy::transparent_production
	{
		static constexpr auto rule = dsl::recurse<struct Expression>;
		static constexpr auto value = lexy::forward<fz::Expression>;
	};

	struct Expression : lexy::expression_production 
	{
		struct Err_ExpectedOperand {
			static constexpr auto name = "ExpectedOperand";
		};

		static constexpr auto atom = [] {
			auto parenthesized = dsl::parenthesized(dsl::p<NestedExpression>);
			auto primitive = dsl::p<PrimitiveExpression>;
			return parenthesized | primitive
				| dsl::error<Err_ExpectedOperand>;
		}();

		struct ProductExpression : dsl::infix_op_left
		{
			static constexpr auto op 
					= dsl::op<fz::BinaryOperator::multiply>(LEXY_LIT("*"))
					/ dsl::op<fz::BinaryOperator::divide>(LEXY_LIT("/"))
					/ dsl::op<fz::BinaryOperator::modulo>(LEXY_LIT("%"));
			using operand = dsl::atom;
		};

		struct SumExpression : dsl::infix_op_left
		{
			static constexpr auto op 
				= dsl::op<fz::BinaryOperator::add>(LEXY_LIT("+"))
				/ dsl::op<fz::BinaryOperator::subtract>(LEXY_LIT("-"));

			using operand = ProductExpression;
		};

		struct ComparisonExpression : dsl::infix_op_single
		{
			static constexpr auto op
				= dsl::op<fz::BinaryOperator::less_than>(LEXY_LIT("<"))
			//	/ dsl::op<fz::BinaryOperator::less_than_or_equal>(LEXY_LIT("<="))
				/ dsl::op<fz::BinaryOperator::greater_than>(LEXY_LIT(">"))
			//	/ dsl::op<fz::BinaryOperator::greater_than_or_equal>(LEXY_LIT(">="))
				/ dsl::op<fz::BinaryOperator::equal>(LEXY_LIT("=="))
				/ dsl::op<fz::BinaryOperator::not_equal>(LEXY_LIT("!="));
			using operand = SumExpression;
		};

		struct AssignmentStatement : dsl::infix_op_single
		{
			static constexpr auto op = dsl::op<fz::Assignment_t>(LEXY_LIT(":="));
			using operand = SumExpression;
		};

		struct KeywordExpression : dsl::prefix_op
		{
			static constexpr auto op 
				= dsl::op<fz::PrefixKeyword::return_>(LEXY_LIT("return"))
				/ dsl::op<fz::PrefixKeyword::let>(LEXY_LIT("let"))
				/ dsl::op<fz::PrefixKeyword::temp>(LEXY_LIT("temp"));
			using operand = AssignmentStatement;
		};

		using operation = KeywordExpression;

		static constexpr auto whitespace 
			= dsl::ascii::blank 
			| (dsl::backslash >> dsl::newline);
		
		static constexpr auto value = lexy::callback<fz::Expression>(
			[](fz::Expression lhs, fz::BinaryOperator op, fz::Expression rhs) -> fz::Expression {
				return fz::BinaryOperation(lhs, op, rhs);
			},
			[](fz::PrefixKeyword kw, fz::Expression rhs) -> fz::Expression {
				return fz::KeywordOperation(kw, rhs);
			},
			[](fz::Expression lhs, fz::Assignment_t, fz::Expression rhs) -> fz::Expression {
				return fz::AssignOperation(lhs, rhs);
			},
			lexy::forward< fz::Expression >
		);
	};
}

