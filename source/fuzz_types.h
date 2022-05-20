#pragma once
#include <utility/value_ptr.h>
#include <optional>
#include <variant>
#include <vector>
#include <string>
#include <map>

namespace fuzz
{
	using string_t = std::u8string;

	struct Object_base;
	using Object = stdish::value_ptr<Object_base>;

	struct Expression_base;
	using Expression = stdish::value_ptr<Expression_base>;

	struct Statement_base;
	using Statement = stdish::value_ptr<Statement_base>;

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
		logical_or
	};

	enum class AssignmentQualifier {
		null, 
		temp, 
		let, 
		require
	};

	// PRIMITIVES

	using Boolean = bool;
	using Number = double;
	using String = string_t;
	using Array = std::vector<Expression>;
	using Block = struct {
		std::vector<Statement> statements;
	};
	using Lambda = struct {
		std::vector<string_t> free_variables;
		Block block;
	};
	using Identifier = struct {
		string_t id;
	};
	using Primitive = std::variant<
		Boolean,
		Number,
		String,
		//Array,
		//Block,
		//Lambda,
		Identifier
	>;

	// EXPRESSIONS

	struct Expression_base{
		virtual ~Expression_base() = default;
	};

	struct BinaryOperation : public Expression_base {
		BinaryOperator operation;
		Expression lhs;
		Expression rhs;
	};
	
	struct Evaluation : public Expression_base{
		std::variant<Identifier, Block> to_evaluate;
		std::vector<Expression> parameter_pack;
	};

	struct PrimitiveExpression : public Expression_base {
		Primitive primitive;

		auto operator<=>(PrimitiveExpression const& other) const = default;
	};

	// STATEMENTS

	struct Statement_base {
		virtual ~Statement_base() = default;
	};
	struct AssignmentStatement : Statement_base {
		AssignmentQualifier qualifier;
		Identifier name;
		Expression expression;
	};
	struct ReturnStatement : Statement_base {
		Expression expression;
	};
	struct ExpressionStatement : Statement_base {
		Expression expression;
	};

	using Program = Block;
}
