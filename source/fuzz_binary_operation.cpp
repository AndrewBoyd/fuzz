#include "fuzz_binary_operation.h"

namespace fuzz 
{
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

