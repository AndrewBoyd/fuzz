#include "fuzz_primitive_expression.h"

namespace fuzz 
{
	template<typename primitive_t>
	Primitive evaluatePrimitive(EvaluationContext const& context, primitive_t primitive) {
		return primitive;
	}

	template<>
	Primitive evaluatePrimitive(EvaluationContext const& context, Identifier identfier) {
		return context.get(identfier);
	}

	Primitive PrimitiveExpression::evaluate(EvaluationContext const& context) const
	{
		return std::visit([&context](auto primitive) {
			return evaluatePrimitive(context, primitive);
			}, primitive);
	}
}

