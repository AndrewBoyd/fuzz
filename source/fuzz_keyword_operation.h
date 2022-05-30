#pragma once
#include "fuzz_types.h"

namespace fuzz
{

	struct KeywordOperation : public Expression_base
	{
		explicit KeywordOperation(PrefixKeyword kw, Expression rhs)
			: Expression_base()
			, keyword(kw)
			, expr(std::move(rhs))
		{};

		void execute( EvaluationContext & ) const override;

		PrefixKeyword keyword;
		Expression expr;
	};
}
