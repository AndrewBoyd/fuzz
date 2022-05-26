#pragma once
#include "fuzz_types.h"

namespace fuzz
{
	class BinaryOperation : public Expression_base
	{
	public:
		explicit BinaryOperation(Expression lhs, BinaryOperator op, Expression rhs)
			: Expression_base()
			, operation_(op)
			, lhs_(std::move(lhs))
			, rhs_(std::move(rhs))
		{}

		Primitive evaluate(EvaluationContext const&) const override;

		BinaryOperator operation_;
		Expression lhs_;
		Expression rhs_;
	};
}
