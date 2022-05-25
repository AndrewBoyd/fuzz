#include "fuzz_types.h"
#include <utility/string_utils.h>

namespace fuzz 
{
	Primitive& EvaluationContext::get(Identifier id) 
	{
		// TODO: Fuzzy Matching
		for (auto object : context_) 
		{
			auto& values = object.values;
			if (auto found_it = values.find(id.id);
				found_it != values.end())
			{
				return found_it->second;
			}
		}

		throw "Unknown identifier " + string_utils::to_ascii(id.id);
	}

	Primitive const& EvaluationContext::get(Identifier id) const 
	{
		return (const_cast<EvaluationContext*>(this))->get(id);
	}

	//////////////////////////////////////////////////////
	//////////////////////////////////////////////////////

	template<typename primitive_t>
	Primitive evaluatePrimitive(EvaluationContext const& context, primitive_t primitive) {
		return primitive;
	}

	template<>
	Primitive evaluatePrimitive(EvaluationContext const& context, Identifier identfier) {
		return context.get(identfier);
	}

	Primitive PrimitiveExpression::evaluate(EvaluationContext const & context) const 
	{
		return std::visit([&context](auto primitive) {
			return evaluatePrimitive(context, primitive);
		}, primitive);
	}

	//////////////////////////////////////////////////////
	//////////////////////////////////////////////////////

	template<typename lhs_t, typename rhs_t>
	Primitive evaluateOperatorAdd(lhs_t lhs, rhs_t rhs) {
		throw "incompatible types for +";
	}

	template<>
	Primitive evaluateOperatorAdd(Number lhs, Number rhs) {
		return lhs + rhs;
	}

	template<>
	Primitive evaluateOperatorAdd(String lhs, String rhs) {
		return lhs + rhs;
	}

	template<>
	Primitive evaluateOperatorAdd(Boolean lhs, Boolean rhs) {
		return static_cast<Boolean>( lhs ^ rhs );
	}

	template<>
	Primitive evaluateOperatorAdd(Array lhs, Array rhs) {
		 lhs.insert(lhs.end(), rhs.begin(), rhs.end());
		 return lhs;
	}

	Primitive BinaryOperation::evaluate(EvaluationContext const& context) const
	{
		auto lhs = lhs_->evaluate(context);
		auto rhs = rhs_->evaluate(context);

		return std::visit([this, &context](auto lhs, auto rhs) {
			switch (operation_) 
			{
			case BinaryOperator::add:
				return evaluateOperatorAdd(lhs, rhs);
			default:
				throw "Unkown Binary operator.";
			}
		}, lhs, rhs);
	}

}

