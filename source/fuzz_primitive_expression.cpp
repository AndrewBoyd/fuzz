#include "fuzz_primitive_expression.h"

namespace fuzz 
{
	template<typename primitive_t>
	Primitive evaluatePrimitive(EvaluationContext & context, primitive_t primitive) {
		return primitive;
	}

	template<>
	Primitive evaluatePrimitive(EvaluationContext & context, Identifier identfier) {
		return context.get(identfier);
	}

	template<>
	Primitive evaluatePrimitive(EvaluationContext& context, Array arr) {
		for (auto & x : arr) {
			x = PrimitiveExpression( x->evaluate(context) );
		}
		return arr;
	}

	Primitive PrimitiveExpression::evaluate(EvaluationContext & context) const
	{
		return std::visit([&context](auto primitive) {
			return evaluatePrimitive(context, primitive);
		}, primitive);
	}
}

