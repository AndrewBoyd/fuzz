#pragma once
#include "fuzz_types.h"

namespace fuzz
{
	class EvaluationContext 
	{
	public:

		Primitive& get(Identifier id);
		Primitive const & get(Identifier id) const;

		void markReturned();
		void setVariable(Identifier id, Primitive primitive);
		TransientObject& getCurrentObject();

	protected:
		Primitive* find(Identifier id);
		Primitive const * find(Identifier id) const;

	private:
		std::list<TransientObject> object_stack_;
	};
}
