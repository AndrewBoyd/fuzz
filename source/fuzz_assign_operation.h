#pragma once
#include "fuzz_types.h"

namespace fuzz
{
	struct AssignOperation : public Expression_base
	{
		explicit AssignOperation(Expression l, Expression r)
			: Expression_base()
			, lhs(std::move(l))
			, rhs(std::move(r))
		{};

		Expression lhs;
		Expression rhs;
	};

}
