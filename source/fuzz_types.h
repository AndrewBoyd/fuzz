#pragma once
#include <utility/value_ptr.h>
#include <optional>
#include <variant>
#include <vector>
#include <string>
#include <exception>
#include <any>
#include <map>
#include <set>

namespace fuzz
{
	using string_t = std::u8string;
	
	class EvaluationContext;

	struct Expression_base;
	using Expression = stdish::value_ptr<Expression_base>;
	class PrimitiveExpression;

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

	struct Lambda;
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

	struct Lambda {
		std::vector<Identifier> free_parameters;
		Object bound_parameters;
		Block block;
	};

	// EXPRESSIONS

	struct Expression_base{
		virtual ~Expression_base() = default;
		
		virtual Primitive evaluate(EvaluationContext&) const
		{
			throw std::exception("Expression does not return a value");
		}

		virtual void execute(EvaluationContext&) const
		{
			throw std::exception("Expression is not a statement");
		}

		virtual bool canExecute() const
		{
			return false;
		};
	};


	using Program = Block;


	template< typename primitive_t > inline
	std::string tname(primitive_t type) { return typeid(type).name(); }
	template<> inline std::string tname(Primitive) { return "Primitive"; }
	template<> inline std::string tname(Array) { return "Array"; }
	template<> inline std::string tname(Number) { return "Number"; }
	template<> inline std::string tname(String) { return "String"; }
	template<> inline std::string tname(Boolean) { return "Boolean"; }
}

#include "fuzz_evaluation_context.h"

#include "fuzz_evaluation.h"
#include "fuzz_binary_operation.h"
#include "fuzz_primitive_expression.h"
#include "fuzz_assign_operation.h"
#include "fuzz_keyword_operation.h"


