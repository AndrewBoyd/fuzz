#include "fuzz_evaluation.h"
#include <fmt/format.h>

namespace fuzz 
{

	Primitive evaluateRecursive(EvaluationContext&, Primitive lhs, Primitive rhs);

	////////////////////////////////////////////

	template <typename primitive_t>
	Lambda bindParameter(Lambda const& f, primitive_t x)
	{
		auto g = f;
		auto& free = g.free_parameters;
		auto& bound = g.bound_parameters;
		auto param = free.front();
		
		bound.values[param.id] = x;
		free = std::vector<Identifier>(free.begin() + 1, free.end());
		return g;
	}

	Primitive executeFunction(EvaluationContext & context, Lambda const & f) 
	{
		if (!f.free_parameters.empty())
			throw std::exception("Attempting to execute a function with free parameters remaining");

		context.pushObject(f.bound_parameters);
		
		for (auto & statement : f.block.statements) {
			statement->execute(context);
		}
		
		return context.popObject();
	}

	////////////////////////////////////////////

	template <typename lhs_t, typename rhs_t>
	Primitive evaluation(EvaluationContext &, lhs_t lhs, rhs_t rhs) 
	{
		auto error_description = fmt::format("incompatible type: {} . {}",
			tname(lhs), tname(rhs));
		throw std::exception(error_description.c_str());
	}

	template <typename primitive_t>
	Primitive evaluation(EvaluationContext& context, primitive_t f, Array xs)
	{
		auto result = Primitive(f);
		for (auto x : xs)
		{
			auto x_eval = x->evaluate(context);
			result = evaluateRecursive(context, result, x_eval);
		}
		return result;
	}

	Primitive evaluationSingleParam(EvaluationContext & context, Lambda f, auto x) 
	{
		auto g = bindParameter(f, x);
		if (g.free_parameters.empty()) {
			return executeFunction(context, g);
		}
		return g;
	}

	Primitive evaluation(EvaluationContext& context, Lambda f, Number x) {
		return evaluationSingleParam(context, std::move(f), std::move(x));
	}

	Primitive evaluation(EvaluationContext& context, Lambda f, String x) {
		return evaluationSingleParam(context, std::move(f), std::move(x));
	}

	Primitive evaluation(EvaluationContext& context, Lambda f, Object x) {
		return evaluationSingleParam(context, std::move(f), std::move(x));
	}

	Primitive evaluation(EvaluationContext& context, Lambda f, Boolean x) {
		return evaluationSingleParam(context, std::move(f), std::move(x));
	}

	Primitive evaluation(EvaluationContext& context, Lambda f, Identifier x) {
		return evaluationSingleParam(context, std::move(f), std::move(x));
	}

	Primitive evaluation(EvaluationContext& context, Lambda f, Lambda x) {
		return evaluationSingleParam(context, std::move(f), std::move(x));
	}

	Primitive evaluation(EvaluationContext& context, Array arr, Number x) 
	{
		auto modded = fmod(x, arr.size());
		auto index = static_cast<size_t>(std::round(x));
		return arr.at(index)->evaluate( context );
	}

	Primitive evaluation(EvaluationContext& context, Object obj, String member)
	{
		return obj.values[member];
	}

	Primitive evaluateRecursive(EvaluationContext& context, Primitive lhs, Primitive rhs)
	{
		return std::visit([&context](auto lhs, auto rhs)
		{
			return evaluation(context, lhs, rhs);
		}, lhs, rhs);
	}

	Primitive Evaluation::evaluate(EvaluationContext & context) const
	{
		auto lhs = to_evaluate->evaluate(context);
		auto rhs = parameter_pack->evaluate(context);
		return evaluateRecursive(context, lhs, rhs);
	}
}

