#include "fuzz_binary_operation.h"
#include <utility/string_utils.h>
#include <utility/concat.h>
#include <utility/guid.h>
#include <fmt/format.h>

namespace fuzz 
{
	Primitive evaluate(EvaluationContext const& context, Primitive lhs, Primitive rhs, BinaryOperator op);
	Primitive evaluate(EvaluationContext const& context, Expression lhs, Expression rhs, BinaryOperator op);

	template< typename primitive_t >
	std::string tname(primitive_t type) { return typeid(type).name(); }

	template<>
	std::string tname(Primitive) { return "Primitive"; }

	////////////////////////////////////////////////
	////////////////////////////////////////////////
	////////////////////////////////////////////////

	template<typename lhs_t, typename rhs_t>
	Primitive evaluateOperatorAdd(EvaluationContext const& context, lhs_t lhs, rhs_t rhs) {
		auto error_description = fmt::format("incompatible type: {} + {}",
			tname(lhs), tname(rhs));
		throw std::exception(error_description.c_str());
	}

	template<>
	Primitive evaluateOperatorAdd(EvaluationContext const& context, Number lhs, Number rhs) {
		return lhs + rhs;
	}

	template<>
	Primitive evaluateOperatorAdd(EvaluationContext const& context, String lhs, String rhs) {
		return lhs + rhs;
	}

	template<>
	Primitive evaluateOperatorAdd(EvaluationContext const& context, String lhs, Number rhs) {
		return lhs + string_utils::to_u8(fmt::format("{.2f}", rhs));
	}

	template<>
	Primitive evaluateOperatorAdd(EvaluationContext const& context, String lhs, Boolean rhs) {
		return lhs + string_utils::to_u8(fmt::format("{}", rhs));
	}

	template<>
	Primitive evaluateOperatorAdd(EvaluationContext const& context, Boolean lhs, Boolean rhs) {
		return static_cast<Boolean>( lhs ^ rhs );
	}

	template<>
	Primitive evaluateOperatorAdd(EvaluationContext const& context, Array lhs, Array rhs) {
		 lhs.insert(lhs.end(), rhs.begin(), rhs.end());
		 return lhs;
	}
	////////////////////////////////////////////////
	////////////////////////////////////////////////
	////////////////////////////////////////////////

	template<typename lhs_t, typename rhs_t>
	Primitive evaluateOperatorSubtract(EvaluationContext const& context, lhs_t lhs, rhs_t rhs) {
		auto error_description = fmt::format("incompatible type: {} - {}",
			tname(lhs), tname(rhs));
		throw std::exception(error_description.c_str());
	}

	template<>
	Primitive evaluateOperatorSubtract(EvaluationContext const& context, Number lhs, Number rhs) {
		return lhs - rhs;
	}

	template<>
	Primitive evaluateOperatorSubtract(EvaluationContext const& context, String lhs, String rhs) {
		return lhs + u8"-" + rhs;
	}

	template<>
	Primitive evaluateOperatorSubtract(EvaluationContext const& context, String lhs, Number rhs) {
		auto to_remove = std::min((size_t)rhs, lhs.size());
		lhs.erase(lhs.end() - to_remove, lhs.end());
		return lhs;
	}

	template<>
	Primitive evaluateOperatorSubtract(EvaluationContext const& context, Boolean lhs, Boolean rhs) {
		return static_cast<Boolean>(lhs ^ rhs);
	}

	////////////////////////////////////////////////
	////////////////////////////////////////////////
	////////////////////////////////////////////////

	template<typename lhs_t, typename rhs_t>
	Primitive evaluateOperatorMultiply(EvaluationContext const& context, lhs_t lhs, rhs_t rhs) {
		auto error_description = fmt::format("incompatible type: {} * {}",
			tname(lhs), tname(rhs));
		throw std::exception(error_description.c_str());
	}

	template<>
	Primitive evaluateOperatorMultiply(EvaluationContext const& context, Number lhs, Number rhs) {
		return lhs * rhs;
	}

	template<>
	Primitive evaluateOperatorMultiply(EvaluationContext const& context, Array lhs, Number rhs) {
		auto as_int = static_cast<int>(rhs);
		if (as_int != rhs)
			return Array{};

		auto multiplied = std::vector<Array>(as_int, lhs);
		return stdish::concat(std::move(multiplied));
	}

	template<>
	Primitive evaluateOperatorMultiply(EvaluationContext const& context, Number lhs, Array rhs) {
		return evaluateOperatorMultiply(context, rhs, lhs);
	}

	template<>
	Primitive evaluateOperatorMultiply(EvaluationContext const& context, String lhs, Number rhs) {
		auto as_int = static_cast<int>(rhs);
		if (as_int != rhs)
			return Array{};

		auto multiplied = std::vector<String>(as_int, lhs);
		return string_utils::concat(std::move(multiplied), "");
	}

	template<>
	Primitive evaluateOperatorMultiply(EvaluationContext const& context, Number lhs, String rhs) {
		return evaluateOperatorMultiply(context, rhs, lhs);
	}

	template<>
	Primitive evaluateOperatorMultiply(EvaluationContext const& context, Boolean lhs, Boolean rhs) {
		return lhs && rhs;
	}

	template<>
	Primitive evaluateOperatorMultiply(EvaluationContext const& context, Array lhs, Array rhs) {
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
	Primitive evaluateOperatorDivide(EvaluationContext const& context, lhs_t lhs, rhs_t rhs) {
		auto error_description = fmt::format("incompatible type: {} / {}",
			tname(lhs), tname(rhs));
		throw std::exception(error_description.c_str());
	}

	template<>
	Primitive evaluateOperatorDivide(EvaluationContext const& context, Number lhs, Number rhs) {
		return lhs / rhs;
	}

	template<>
	Primitive evaluateOperatorDivide(EvaluationContext const& context, Array lhs, Number rhs) {
		auto as_int = static_cast<int>(rhs);
		auto snip = lhs.size() / as_int;
		lhs.erase(lhs.begin() + snip, lhs.end());
		return lhs;
	}

	template<>
	Primitive evaluateOperatorDivide(EvaluationContext const& context, String lhs, Number rhs) {
		auto as_int = static_cast<int>(rhs);
		auto snip = lhs.size() / as_int;
		lhs.erase(lhs.begin() + snip, lhs.end());
		return lhs;
	}

	template<>
	Primitive evaluateOperatorDivide(EvaluationContext const& context, Boolean lhs, Boolean rhs) {
		return lhs == rhs;
	}

	template<>
	Primitive evaluateOperatorDivide(EvaluationContext const& context, Array lhs, Array rhs) {
		return Array{};
	}

	////////////////////////////////////////////////
	////////////////////////////////////////////////
	////////////////////////////////////////////////

	template<typename lhs_t, typename rhs_t>
	Primitive evaluateOperatorModulo(EvaluationContext const& context, lhs_t lhs, rhs_t rhs) {
		auto error_description = fmt::format("incompatible type: {} % {}",
			tname(lhs), tname(rhs));
		throw std::exception(error_description.c_str());
	}

	template<>
	Primitive evaluateOperatorModulo(EvaluationContext const& context, Number lhs, Number rhs) {
		return std::fmod(lhs, rhs);
	}

	template<>
	Primitive evaluateOperatorModulo(EvaluationContext const& context, Array lhs, Number rhs) {
		auto as_int = static_cast<int>(rhs);
		auto snip = lhs.size() / as_int;
		lhs.erase(lhs.begin(), lhs.begin() + snip);
		return lhs;
	}

	template<>
	Primitive evaluateOperatorModulo(EvaluationContext const& context, String lhs, Number rhs) {
		auto as_int = static_cast<int>(rhs);
		auto snip = lhs.size() / as_int;
		lhs.erase(lhs.begin(), lhs.begin() + snip);
		return lhs;
	}

	template<>
	Primitive evaluateOperatorModulo(EvaluationContext const& context, Boolean lhs, Boolean rhs) {
		return lhs != rhs;
	}

	template<>
	Primitive evaluateOperatorDivide(EvaluationContext const& context, Array lhs, Array rhs) {
		return Array{};
	}

	////////////////////////////////////////////////
	////////////////////////////////////////////////
	////////////////////////////////////////////////

	Primitive evaluate(EvaluationContext const& context, Primitive lhs, Primitive rhs, BinaryOperator op)
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
			default:
				throw std::exception("Unknown Binary operator.");
			}
			}, lhs, rhs);
	}

	Primitive evaluate(EvaluationContext const& context, Expression lhs, Expression rhs, BinaryOperator op)
	{
		auto l = lhs->evaluate(context);
		auto r = rhs->evaluate(context);
		return evaluate(context, l, r, op);
	}

	Primitive BinaryOperation::evaluate(EvaluationContext const& context) const
	{
		return fuzz::evaluate(context, lhs_, rhs_, operation_);
	}

}

