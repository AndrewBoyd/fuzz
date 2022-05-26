#pragma once
#include "fuzz_types.h"

namespace fuzz
{
	struct PrimitiveExpression : public Expression_base
	{
		Primitive primitive;

		Primitive evaluate(EvaluationContext const&) const override;
	};

}
