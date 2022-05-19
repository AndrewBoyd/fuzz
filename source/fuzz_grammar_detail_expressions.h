#pragma once

namespace fuzz_grammar
{
	struct NestedExpression : lexy::transparent_production
	{
		static constexpr auto rule = dsl::recurse<struct Expression>;
		static constexpr auto value = lexy::forward<fz::Expression>;
	};

	struct PrimitiveExpression : lexy::transparent_production
	{
		static constexpr auto rule = dsl::p<Primitive>;
		static constexpr auto value = [](fz::Primitive p) -> fz::Expression{
			return fz::PrimitiveExpression{
				.primitive = p
			};
		};
	};

	struct Expression : lexy::expression_production 
	{
		struct Err_ExpectedOperand {
			static constexpr auto name = "ExpectedOperand";
		};

		static constexpr auto atom = [] {
			auto parenthesized 
				= dsl::parenthesized(dsl::p<NestedExpression>);
			//auto var_or_call = dsl::p<name> >> dsl::if_(paren_expr);
			auto primitive = dsl::p<PrimitiveExpression>;

			return parenthesized 
				| primitive
				| dsl::error<Err_ExpectedOperand>;
		}();

		struct ProductExpression : dsl::infix_op_left
		{
			static constexpr auto op = [] {
				return dsl::op<fz::BinaryOperator::multiply>(LEXY_LIT("*"))
					/ dsl::op<fz::BinaryOperator::divide>(LEXY_LIT("/"))
					/ dsl::op<fz::BinaryOperator::modulo>(LEXY_LIT("%"));
			}();
			using operand = Expression;
		};

		struct SumExpression : dsl::infix_op_left
		{
			static constexpr auto op = []{
				return dsl::op<fz::BinaryOperator::add>(LEXY_LIT("+"))
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
				/ dsl::op<fz::BinaryOperator::not_equal>(LEXY_LIT("!="))
			;
			using operand = SumExpression;
		};

		using operation = ComparisonExpression;

		//static constexpr auto value =
		//	// We need a sink as the comparison expression generates a list.
		//	lexy::fold_inplace<std::unique_ptr<ast::expr_comparison>>(
		//		[] { return std::make_unique<ast::expr_comparison>(); },
		//		[](auto& node, ast::expr_ptr opr) { node->operands.push_back(LEXY_MOV(opr)); },
		//		[](auto& node, ast::expr_comparison::op_t op) { node->ops.push_back(op); })
		//	// The result of the list feeds into a callback that handles all other cases.
		//	>> lexy::callback(
		//		// atoms
		//		lexy::forward<ast::expr_ptr>, lexy::new_<ast::expr_literal, ast::expr_ptr>,
		//		lexy::new_<ast::expr_name, ast::expr_ptr>, lexy::new_<ast::expr_call, ast::expr_ptr>,
		//		// unary/binary operators
		//		lexy::new_<ast::expr_unary_arithmetic, ast::expr_ptr>,
		//		lexy::new_<ast::expr_binary_arithmetic, ast::expr_ptr>,
		//		// conditional and assignment
		//		lexy::new_<ast::expr_if, ast::expr_ptr>,
		//		lexy::new_<ast::expr_assignment, ast::expr_ptr>);

		static constexpr auto value = lexy::callback(
			lexy::forward< fz::Expression >,
			lexy::new_< fz::PrimitiveExpression, fz::Expression >,
			lexy::new_< fz::BinaryOperation, fz::Expression >
		);
	};
}

