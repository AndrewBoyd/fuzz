#include "fuzz_keyword_operation.h"

namespace fuzz 
{
	void KeywordOperation::execute(EvaluationContext& context) const
	{
		switch (keyword) {
		case PrefixKeyword::return_:
		{
			context.returnPrimitive(expr->evaluate(context));
			break;
		}
		default:
			throw std::exception("Unknown keyword");
		};
	}
}

