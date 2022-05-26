#pragma once
#include <utility/value_ptr.h>
#include <optional>
#include <variant>
#include <vector>
#include <string>
#include <any>
#include <map>
#include <set>

namespace fuzz
{
	using string_t = std::u8string;
	
	class EvaluationContext;

	struct Expression_base;
	using Expression = stdish::value_ptr<Expression_base>;
	struct PrimitiveExpression;

	enum class BinaryOperator {
		add, 
		subtract, 
		multiply, 
		divide, 
		modulo,
		less_than, 
		less_than_or_equal, 
		greater_than, 
		greater_than_or_equal, 
		equal, 
		not_equal,
		logical_and, 
		logical_or,
		evaluate,
	};

	enum class PrefixKeyword {
		return_,
		temp, 
		let,
	};

	struct Assignment_t {};
	struct Evaluation_t {};

	// PRIMITIVES

	using Boolean = bool;
	using Number = double;
	using String = string_t;
	using Array = std::vector<Expression>;
	using Block = struct {
		std::vector<Expression> statements;
	};
	using Identifier = struct {
		string_t id;
	};
	using Lambda = struct {
		std::vector<Identifier> free_parameters;
		Block block;
	};

	struct Object;

	using Primitive = std::variant<
		Boolean,
		Number,
		String,
		Array,
		Lambda,
		Object,
		Identifier
	>;

	struct Object {
		std::map<string_t, Primitive > values;
	};

	// EXPRESSIONS

	struct Expression_base{
		virtual ~Expression_base() = default;
		
		virtual Primitive evaluate(EvaluationContext const &) const
		{
			throw "Expression does not return a value";
		}
		
		virtual void execute(EvaluationContext&) const
		{
			throw "Expression is not a statement";
		}
	};


	using Program = Block;
}

#include "fuzz_evaluation_context.h"

#include "fuzz_evaluation.h"
#include "fuzz_binary_operation.h"
#include "fuzz_primitive_expression.h"
#include "fuzz_assign_operation.h"
#include "fuzz_keyword_operation.h"


