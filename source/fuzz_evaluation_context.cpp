#include "fuzz_evaluation_context.h"
#include <utility/string_utils.h>

namespace fuzz 
{
	EvaluationContext::EvaluationContext()
	{
		object_stack_.push_front(TransientObject{});
	}

	Primitive& EvaluationContext::get(Identifier id)
	{
		if (auto* found = find(id))
			return *found;
		auto exception_string = "Unknown identifier " + string_utils::to_ascii(id.id);
		throw std::exception(exception_string.c_str());
	}

	Primitive const& EvaluationContext::get(Identifier id) const 
	{
		return (const_cast<EvaluationContext*>(this))->get(id);
	}

	void EvaluationContext::returnPrimitive(Primitive primitive)
	{
		getCurrentObject().returned_value = primitive;
	}

	void EvaluationContext::setVariable(Identifier id, Primitive primitive)
	{
		if (auto* found = find(id)) {
			*found = std::move(primitive);
		}
		else {
			getCurrentObject().values[id.id] = std::move(primitive);
		}
	}

	TransientObject& EvaluationContext::getCurrentObject()
	{
		return object_stack_.front();
	}

	void EvaluationContext::pushObject(Object object)
	{
		object_stack_.push_front(TransientObject(std::move(object)));
	}

	Primitive EvaluationContext::popObject()
	{
		auto result = object_stack_.front();
		object_stack_.pop_front();

		if (result.returned_value)
			return *result.returned_value;

		auto& vs = result.values;
		for (auto temp_name : result.temp_names) {
			if (vs.find(temp_name.id) != vs.end()) {
				vs.erase(temp_name.id);
			}
		}
		
		return static_cast<Object&>(result);
	}

	Primitive* EvaluationContext::find(Identifier id)
	{
		// TODO: Fuzzy Matching
		for (auto& object : object_stack_)
		{
			auto& values = object.values;
			if (auto found_it = values.find(id.id);
				found_it != values.end())
			{
				return &found_it->second;
			}
		}
		return nullptr;
	}

	Primitive const* EvaluationContext::find(Identifier id) const 
	{
		// Don't want to type this all again :)
		return (const_cast<EvaluationContext*>(this))->find(id);
	}

	//////////////////////////////////////////////////////
	//////////////////////////////////////////////////////


}

