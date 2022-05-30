#include "fuzz_evaluation.h"


namespace fuzz 
{
	template <typename primitive_t>
	Lambda bind(Lambda const& f, primitive_t x) 
	{
		auto g = f;
		auto& free = g.free_parameters;
		auto& bound = g.bound_parameters;
		auto param = free.front();
		
		bound[param.id] = x;
		free = std::vector<Identifier>(free.begin() + 1, free.end());
		return g;
	}

	Primitive executeFunction(EvaluationContext & context, Lambda const & f) 
	{
		if (!f.free_parameters.empty())
			throw std::exception("Attempting to execute a function with free parameters remaining");

		context.pushObject(f.bound_parameters);
		auto expr = PrimitiveExpression(f);
		
		//auto result = expr.evaluate(context);
		
		context.popObject();
		return result;
	}


	////////////////////////////////////////////

	template <typename primitive_t>
	Primitive evaluation(EvaluationContext &, primitive_t lhs, primitive_t rhs) 
	{
		auto error_description = fmt::format("incompatible type: {} . {}",
			tname(lhs), tname(rhs));
		throw std::exception(error_description.c_str());
	}

	template <typename primitive_t>
	Primitive evaluation(EvaluationContext & context, Lambda const&f, primitive_t x) 
	{
		auto g = bind(f, x);
		if (g.free_parameters.empty()) {
			return executeFunction(context, g);
		}
		return g;
	}

	Primitive Evaluation::evaluate(EvaluationContext & context) const
	{
		return std::visit([&context](auto & lhs, auto & rhs) 
		{
			auto lhs_eval = lhs.evaluate(context);
			auto rhs_eval = rhs.evaluate(context);
			return evaluation(context, lhs_eval, rhs_eval);
		}, to_evaluate, parameter_pack);
	}
}

