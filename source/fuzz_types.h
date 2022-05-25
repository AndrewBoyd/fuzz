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

	enum class PrefixKeyword {
		return_,
		temp, 
		let,
	};

	struct Assignment_t {};

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
	using Primitive = std::variant<
		Boolean,
		Number,
		String,
		Array,
		Block,
		Lambda,
		Identifier
	>;

	// EXPRESSIONS

	struct Expression_base{
		virtual ~Expression_base() = default;
	};

	struct BinaryOperation : public Expression_base 
	{
		explicit BinaryOperation(Expression l, BinaryOperator op, Expression r) 
			: Expression_base()
			, operation(op)
			, lhs(std::move(l))
			, rhs(std::move(r))
		{}

		BinaryOperator operation;
		Expression lhs;
		Expression rhs;
	};

	struct KeywordOperation : public Expression_base
	{
		explicit KeywordOperation(PrefixKeyword kw, Expression rhs)
			: Expression_base()
			, keyword(kw)
			, expr(std::move(rhs)) 
		{};

		PrefixKeyword keyword;
		Expression expr;
	};

	struct AssignOperation : public Expression_base
	{
		explicit AssignOperation(Expression l, Expression r)
			: Expression_base()
			, lhs(std::move(l))
			, rhs(std::move(r))
		{};

		Expression lhs;
		Expression rhs;
	};

	struct Evaluation : public Expression_base
	{
		Expression to_evaluate;
		std::vector<Expression> parameter_pack;
	};

	struct PrimitiveExpression : public Expression_base {
		Primitive primitive;

		auto operator<=>(PrimitiveExpression const& other) const = default;
	};

	using Program = Block;
}
