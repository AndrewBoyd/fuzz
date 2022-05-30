#pragma once
#include "fuzz_types.h"

namespace fuzz
{
	class TransientObject : public Object {
	public:
		std::set<Identifier> temp_names = {};
		std::optional<Primitive> returned_value = {};
	};
	
	class EvaluationContext 
	{
	public:
		EvaluationContext();

		Primitive& get(Identifier id);
		Primitive const & get(Identifier id) const;

		void returnPrimitive(Primitive primitive);
		void setVariable(Identifier id, Primitive primitive);
		TransientObject& getCurrentObject();

		void pushObject(Object object);
		Primitive popObject();

	protected:
		Primitive* find(Identifier id);
		Primitive const * find(Identifier id) const;

	private:
		std::list<TransientObject> object_stack_;
	};
}
