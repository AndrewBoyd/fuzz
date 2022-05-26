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

		Expression to_evaluate;
		Expression parameter_pack;
	};
}
