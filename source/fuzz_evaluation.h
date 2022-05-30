#pragma once
#include "fuzz_types.h"

namespace fuzz
{
	struct Evaluation : public Expression_base
	{
		explicit Evaluation(Expression lhs, Expression rhs)
			: Expression_base()
			, to_evaluate(std::move(lhs))
			, parameter_pack(std::move(rhs))
		{};

		Primitive evaluate(EvaluationContext &) const override;

		Expression to_evaluate;
		Expression parameter_pack;
	};
}
