#include "fuzz_binary_operation.h"
#include <utility/string_utils.h>
#include <utility/concat.h>
#include <utility/guid.h>
#include <fmt/format.h>

namespace fuzz 
{
	Primitive evaluate(EvaluationContext & context, Primitive lhs, Primitive rhs, BinaryOperator op);
	Primitive evaluate(EvaluationContext & context, Expression lhs, Expression rhs, BinaryOperator op);

	template<typename primitive_t>
	bool toBoolean_impl(primitive_t) { 
		return false; 
	}

	template<>
	bool toBoolean_impl(bool x) { 
		return x; 
	}
	
	bool toBoolean(Primitive p) {
		return std::visit([](auto x) { 
			return toBoolean_impl(x);
		}, p);
	}

	////////////////////////////////////////////////
	////////////////////////////////////////////////
	////////////////////////////////////////////////

	template<typename lhs_t, typename rhs_t>
	Primitive evaluateOperatorAdd(EvaluationContext & context, lhs_t lhs, rhs_t rhs) {
		auto error_description = fmt::format("incompatible type: {} + {}",
			tname(lhs), tname(rhs));
		throw std::exception(error_description.c_str());
	}

	template<>
	Primitive evaluateOperatorAdd(EvaluationContext & context, Number lhs, Number rhs) {
		return lhs + rhs;
	}

	template<>
	Primitive evaluateOperatorAdd(EvaluationContext & context, String lhs, String rhs) {
		return lhs + rhs;
	}

	template<>
	Primitive evaluateOperatorAdd(EvaluationContext & context, String lhs, Number rhs) {
		return lhs + string_utils::to_u8(fmt::format("{:.2f}", rhs));
	}

	template<>
	Primitive evaluateOperatorAdd(EvaluationContext & context, String lhs, Boolean rhs) {
		return lhs + string_utils::to_u8(fmt::format("{}", rhs));
	}

	template<>
	Primitive evaluateOperatorAdd(EvaluationContext & context, Boolean lhs, Boolean rhs) {
		return static_cast<Boolean>( lhs ^ rhs );
	}

	template<>
	Primitive evaluateOperatorAdd(EvaluationContext & context, Array lhs, Array rhs) {
		 lhs.insert(lhs.end(), rhs.begin(), rhs.end());
		 return lhs;
	}
	////////////////////////////////////////////////
	////////////////////////////////////////////////
	////////////////////////////////////////////////

	template<typename lhs_t, typename rhs_t>
	Primitive evaluateOperatorSubtract(EvaluationContext & context, lhs_t lhs, rhs_t rhs) {
		auto error_description = fmt::format("incompatible type: {} - {}",
			tname(lhs), tname(rhs));
		throw std::exception(error_description.c_str());
	}

	template<>
	Primitive evaluateOperatorSubtract(EvaluationContext & context, Number lhs, Number rhs) {
		return lhs - rhs;
	}

	template<>
	Primitive evaluateOperatorSubtract(EvaluationContext & context, String lhs, String rhs) {
		return lhs + u8"-" + rhs;
	}

	template<>
	Primitive evaluateOperatorSubtract(EvaluationContext & context, String lhs, Number rhs) {
		auto to_remove = std::min((size_t)rhs, lhs.size());
		lhs.erase(lhs.end() - to_remove, lhs.end());
		return lhs;
	}

	template<>
	Primitive evaluateOperatorSubtract(EvaluationContext & context, Array lhs, Number rhs) {
		auto to_remove = std::min((size_t)rhs, lhs.size());
		lhs.erase(lhs.end() - to_remove, lhs.end());
		return lhs;
	}

	template<>
	Primitive evaluateOperatorSubtract(EvaluationContext & context, Boolean lhs, Boolean rhs) {
		return static_cast<Boolean>(lhs ^ rhs);
	}

	////////////////////////////////////////////////
	////////////////////////////////////////////////
	////////////////////////////////////////////////

	template<typename lhs_t, typename rhs_t>
	Primitive evaluateOperatorMultiply(EvaluationContext & context, lhs_t lhs, rhs_t rhs) {
		auto error_description = fmt::format("incompatible type: {} * {}",
			tname(lhs), tname(rhs));
		throw std::exception(error_description.c_str());
	}

	template<>
	Primitive evaluateOperatorMultiply(EvaluationContext & context, Number lhs, Number rhs) {
		return lhs * rhs;
	}

	template<>
	Primitive evaluateOperatorMultiply(EvaluationContext & context, Array lhs, Number rhs) {
		auto as_int = static_cast<int>(rhs);
		if (as_int != rhs)
			return Array{};

		auto multiplied = std::vector<Array>(as_int, lhs);
		return stdish::concat(std::move(multiplied));
	}

	template<>
	Primitive evaluateOperatorMultiply(EvaluationContext & context, Number lhs, Array rhs) {
		return evaluateOperatorMultiply(context, rhs, lhs);
	}

	template<>
	Primitive evaluateOperatorMultiply(EvaluationContext & context, String lhs, Number rhs) {
		auto as_int = static_cast<int>(rhs);
		if (as_int != rhs)
			return Array{};

		auto multiplied = std::vector<String>(as_int, lhs);
		return string_utils::concat(std::move(multiplied), "");
	}

	template<>
	Primitive evaluateOperatorMultiply(EvaluationContext & context, Number lhs, String rhs) {
		return evaluateOperatorMultiply(context, rhs, lhs);
	}

	template<>
	Primitive evaluateOperatorMultiply(EvaluationContext & context, Boolean lhs, Boolean rhs) {
		return lhs && rhs;
	}

	template<>
	Primitive evaluateOperatorMultiply(EvaluationContext & context, Array lhs, Array rhs) {
		auto result = Array{};
		for (auto &l : lhs) {
			auto inner_array = Array{};
			for (auto& r : rhs) {
				auto lr = evaluate(context, l, r, BinaryOperator::multiply);
				inner_array.push_back(PrimitiveExpression(lr));
			}
			result.push_back(PrimitiveExpression(inner_array));
		}
		return result;
	}

	////////////////////////////////////////////////
	////////////////////////////////////////////////
	////////////////////////////////////////////////

	template<typename lhs_t, typename rhs_t>
	Primitive evaluateOperatorDivide(EvaluationContext & context, lhs_t lhs, rhs_t rhs) {
		auto error_description = fmt::format("incompatible type: {} / {}",
			tname(lhs), tname(rhs));
		throw std::exception(error_description.c_str());
	}

	template<>
	Primitive evaluateOperatorDivide(EvaluationContext & context, Number lhs, Number rhs) {
		return lhs / rhs;
	}

	template<>
	Primitive evaluateOperatorDivide(EvaluationContext & context, Array lhs, Number rhs) {
		auto as_int = static_cast<int>(rhs);
		auto snip = lhs.size() / as_int;
		lhs.erase(lhs.begin() + snip, lhs.end());
		return lhs;
	}

	template<>
	Primitive evaluateOperatorDivide(EvaluationContext & context, String lhs, Number rhs) {
		auto as_int = static_cast<int>(rhs);
		auto snip = lhs.size() / as_int;
		lhs.erase(lhs.begin() + snip, lhs.end());
		return lhs;
	}

	template<>
	Primitive evaluateOperatorDivide(EvaluationContext & context, Boolean lhs, Boolean rhs) {
		return lhs == rhs;
	}

	template<>
	Primitive evaluateOperatorDivide(EvaluationContext & context, Array lhs, Array rhs) {
		return Array{};
	}

	////////////////////////////////////////////////
	////////////////////////////////////////////////
	////////////////////////////////////////////////

	template<typename lhs_t, typename rhs_t>
	Primitive evaluateOperatorModulo(EvaluationContext & context, lhs_t lhs, rhs_t rhs) {
		auto error_description = fmt::format("incompatible type: {} % {}",
			tname(lhs), tname(rhs));
		throw std::exception(error_description.c_str());
	}

	template<>
	Primitive evaluateOperatorModulo(EvaluationContext & context, Number lhs, Number rhs) {
		return std::fmod(lhs, rhs);
	}

	template<>
	Primitive evaluateOperatorModulo(EvaluationContext & context, Array lhs, Number rhs) {
		auto as_int = static_cast<int>(rhs);
		auto snip = lhs.size() / as_int;
		lhs.erase(lhs.begin(), lhs.begin() + snip);
		return lhs;
	}

	template<>
	Primitive evaluateOperatorModulo(EvaluationContext & context, String lhs, Number rhs) {
		auto as_int = static_cast<int>(rhs);
		auto snip = lhs.size() / as_int;
		lhs.erase(lhs.begin(), lhs.begin() + snip);
		return lhs;
	}

	template<>
	Primitive evaluateOperatorModulo(EvaluationContext & context, Boolean lhs, Boolean rhs) {
		return lhs != rhs;
	}

	template<>
	Primitive evaluateOperatorModulo(EvaluationContext & context, Array lhs, Array rhs) {
		return Array{};
	}


	////////////////////////////////////////////////
	////////////////////////////////////////////////
	////////////////////////////////////////////////

	bool lt(EvaluationContext & context, Primitive lhs, Primitive rhs) {
		return toBoolean(evaluate(context, lhs, rhs, BinaryOperator::less_than));
	}

	bool lt(EvaluationContext & context, Expression lhs, Expression rhs) {
		return lt(context, lhs->evaluate(context), rhs->evaluate(context));
	}

	bool gt(EvaluationContext & context, auto lhs, auto rhs) {
		return lt(context, rhs, lhs);
	}

	template<typename lhs_t, typename rhs_t>
	Primitive evaluateOperatorLessThan(EvaluationContext & context, lhs_t lhs, rhs_t rhs) {
		// If they aren't comparible, then obviously its not less than.
		return false;
	}

	template<>
	Primitive evaluateOperatorLessThan(EvaluationContext & context, Number lhs, Number rhs) {
		return lhs < rhs;
	}

	template<>
	Primitive evaluateOperatorLessThan(EvaluationContext & context, Array lhs, Array rhs){
		if (lhs.size() != rhs.size())
			return lhs.size() < rhs.size();

		for (auto i = 0u; i < lhs.size(); ++i) {
			if (lt(context, lhs[i], rhs[i])) return true;
			if (gt(context, lhs[i], rhs[i])) return false;
		}

		return false;
	}

	template<>
	Primitive evaluateOperatorLessThan(EvaluationContext & context, String lhs, String rhs) {
		return lhs < rhs;
	}

	template<>
	Primitive evaluateOperatorLessThan(EvaluationContext & context, Boolean lhs, Boolean rhs) {
		return lhs==false && rhs==true;
	}

	////////////////////////////////////////////////
	////////////////////////////////////////////////
	////////////////////////////////////////////////

	template<typename lhs_t, typename rhs_t>
	Primitive evaluateOperatorLessThanOrEqual(EvaluationContext & context, lhs_t lhs, rhs_t rhs) {
		return !gt(context, lhs, rhs);
	}

	template<typename lhs_t, typename rhs_t>
	Primitive evaluateOperatorGreaterThan(EvaluationContext & context, lhs_t lhs, rhs_t rhs) {
		return gt(context, rhs, lhs);
	}

	template<typename lhs_t, typename rhs_t>
	Primitive evaluateOperatorGreaterThanOrEqual(EvaluationContext & context, lhs_t lhs, rhs_t rhs) {
		return !lt(context, lhs, rhs);
	}
	
	template<typename lhs_t, typename rhs_t>
	Primitive evaluateOperatorEqual(EvaluationContext & context, lhs_t lhs, rhs_t rhs) {
		return !lt(context, lhs, rhs) && !gt(context, lhs, rhs);
	}

	template<typename lhs_t, typename rhs_t>
	Primitive evaluateOperatorNotEqual(EvaluationContext & context, lhs_t lhs, rhs_t rhs) {
		return lt(context, lhs, rhs) || gt(context, lhs, rhs);
	}

	template<typename lhs_t, typename rhs_t>
	Primitive evaluateOperatorLogicalAnd(EvaluationContext & context, lhs_t lhs, rhs_t rhs) {
		return toBoolean(lhs) && toBoolean(rhs);
	}

	template<typename lhs_t, typename rhs_t>
	Primitive evaluateOperatorLogicalOr(EvaluationContext & context, lhs_t lhs, rhs_t rhs) {
		return toBoolean(lhs) || toBoolean(rhs);
	}

	////////////////////////////////////////////////
	////////////////////////////////////////////////
	////////////////////////////////////////////////

	Primitive evaluate(EvaluationContext & context, Primitive lhs, Primitive rhs, BinaryOperator op)
	{
		return std::visit([&context, op](auto lhs, auto rhs) {
			switch (op)
			{
			case BinaryOperator::add:
				return evaluateOperatorAdd(context, lhs, rhs);
			case BinaryOperator::subtract:
				return evaluateOperatorSubtract(context, lhs, rhs);
			case BinaryOperator::multiply:
				return evaluateOperatorMultiply(context, lhs, rhs);
			case BinaryOperator::divide:
				return evaluateOperatorDivide(context, lhs, rhs);
			case BinaryOperator::modulo:
				return evaluateOperatorModulo(context, lhs, rhs);
			case BinaryOperator::less_than:
				return evaluateOperatorLessThan(context, lhs, rhs);
			case BinaryOperator::less_than_or_equal:
				return evaluateOperatorLessThanOrEqual(context, lhs, rhs);
			case BinaryOperator::greater_than:
				return evaluateOperatorGreaterThan(context, lhs, rhs);
			case BinaryOperator::greater_than_or_equal:
				return evaluateOperatorGreaterThanOrEqual(context, lhs, rhs);
			case BinaryOperator::equal:
				return evaluateOperatorEqual(context, lhs, rhs);
			case BinaryOperator::not_equal:
				return evaluateOperatorNotEqual(context, lhs, rhs);
			case BinaryOperator::logical_and:
				return evaluateOperatorLogicalAnd(context, lhs, rhs);
			case BinaryOperator::logical_or:
				return evaluateOperatorLogicalOr(context, lhs, rhs);
			default:
				throw std::exception("Unknown Binary operator.");
			}
		}, lhs, rhs);
	}

	Primitive evaluate(EvaluationContext & context, Expression lhs, Expression rhs, BinaryOperator op)
	{
		auto l = lhs->evaluate(context);
		auto r = rhs->evaluate(context);
		return evaluate(context, l, r, op);
	}

	Primitive BinaryOperation::evaluate(EvaluationContext & context) const
	{
		return fuzz::evaluate(context, lhs_, rhs_, operation_);
	}

}

