#pragma once
#include "fuzz_types.h"

namespace fuzz
{
	class PrimitiveExpression : public Expression_base
	{
	public:
		PrimitiveExpression(Primitive p)
			: primitive(std::move(p))
		{}

		Primitive primitive;

		Primitive evaluate(EvaluationContext const&) const override;
	};

}
